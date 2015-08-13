#include "stdhdrs.h"

#include "Fortune_system.h"
#include "Server.h"

struct TEMP_FORTUNE
{
	int				Item_index;
	unsigned char	Prob_type;
	int				Skill_index;
	unsigned char	Skill_level;
	int				Prob;
} Temp_fortune;

Fortune_data::Fortune_data(int _skill_index, int _skill_level, int _prob):
	Skill_index(_skill_index), Skill_level(_skill_level), Prob(_prob)
{
}

Fortune_data::~Fortune_data()
{
}
int Fortune_data::Get_fortune_data_prob()
{
	return Prob;
}

int Fortune_data::Get_fortune_data_skill_index()
{
	return Skill_index;
}

int Fortune_data::Get_fortune_data_skill_level()
{
	return Skill_level;
}

Fortune_head::Fortune_head(int _item_index, int _prob_type):
	Item_index(_item_index), Prob_type(_prob_type)
{
}

Fortune_head::~Fortune_head()
{
	Fortune_data_clear();
}

void Fortune_head::Fortune_data_clear()
{
	Delete_fortune_data_vec();
	fd.clear();
}

void Fortune_head::Delete_fortune_data_vec()
{
	std::vector<Fortune_data*>::iterator itr;
	std::vector<Fortune_data*>::iterator itrEnd = fd.end();
	for (itr = fd.begin(); itr != itrEnd; ++itr)
	{
		Fortune_data* p = *itr;
		if (p)
		{
			delete p;
			p = NULL;
		}
	}
}

std::vector<Fortune_data*> &Fortune_head::Get_Fortune_data()
{
	return fd;
}

int Fortune_head::Get_item_index()
{
	return Item_index;
}

int Fortune_head::Get_prob_type()
{
	return Prob_type;
}

Fortune_proto_list::Fortune_proto_list()
{
}

Fortune_proto_list::~Fortune_proto_list()
{
}

bool Fortune_proto_list::Make_fortune_proto_list(MYSQL *dbdata)
{
	CDBCmd cmd;
	cmd.Init(dbdata);
	CLCString sql(1024);

	int Head_index = 0;
	Fortune_head *h = NULL;
	Fortune_data *d = NULL;

	sql.Format("SELECT "
			   "fh.a_item_idx AS item_idx, "
			   "fh.a_prob_type AS prob_type, "
			   "fd.a_skill_index AS skill_index, "
			   "fd.a_skill_level AS skill_level, "
			   "fd.a_prob AS prob "
			   "FROM t_fortune_head AS fh, t_fortune_data AS fd "
			   "WHERE fh.a_enable = 1 AND fh.a_item_idx = fd.a_item_idx ORDER BY fh.a_item_idx, fd.a_prob");

	cmd.SetQuery(sql);
	if (!cmd.Open())
	{
		GAMELOG << init("ERROR Loading Fortune list.") << end;
		return false;
	}

	if(cmd.m_nrecords == 0)
	{
		GAMELOG << init("NOTICE") << "No data fortune costume" << end;
	}
	else
	{
		cmd.MoveFirst();

		do
		{
			memset(&Temp_fortune, 0, sizeof(Temp_fortune));

			cmd.GetRec("item_idx",		Temp_fortune.Item_index);
			cmd.GetRec("prob_type",		Temp_fortune.Prob_type);
			cmd.GetRec("skill_index",	Temp_fortune.Skill_index);
			cmd.GetRec("skill_level",	Temp_fortune.Skill_level);
			cmd.GetRec("prob",			Temp_fortune.Prob);

			if (Head_index != Temp_fortune.Item_index)
			{
				Head_index = Temp_fortune.Item_index;
				h = new Fortune_head(Temp_fortune.Item_index, (int)Temp_fortune.Prob_type);
				fh.push_back(h);
			}

			d = new Fortune_data(Temp_fortune.Skill_index, (int)Temp_fortune.Skill_level, Temp_fortune.Prob);
			h->Get_Fortune_data().push_back(d);
		}
		while(cmd.MoveNext());
	}
	return true;
}

Fortune_head *Fortune_proto_list::Get_fortune_head_proto(int Item_index)
{
	std::vector<Fortune_head*>::iterator itr;

	Fortune_head *p = new Fortune_head(Item_index, 0);

	itr = std::find_if(fh.begin(), fh.end(), std::bind2nd(Compare_fortune_list(), p));
	if (itr == fh.end())
		return NULL;

	return *itr;
}

Fortune_data *Fortune_proto_list::Get_fortune_result(Fortune_head *f)
{
	if (f == NULL)
		return NULL;

	if (f->Get_prob_type() == Fortune_head::PROB)
	{
		if(!Verify_prob(f))
			return NULL;

		int prob = GetRandom(1, Fortune_head::Sum_of_prob);

		int sum = 0;
		Fortune_data* p = NULL;
		std::vector<Fortune_data*>::iterator itr;
		std::vector<Fortune_data*>::iterator itrEnd = f->Get_Fortune_data().end();
		for (itr = f->Get_Fortune_data().begin(); itr != itrEnd; ++itr)
		{
			p = *itr;
			sum = sum + p->Get_fortune_data_prob();

			if(prob <= sum)
				break;
		}

		if (itr == f->Get_Fortune_data().end())
			return NULL;

		return p;
	}
	else if (f->Get_prob_type() == Fortune_head::RANDOM)
	{
		int pos = GetRandom(0, (f->Get_Fortune_data().size()-1));
		return f->Get_Fortune_data()[pos];
	}
	else
		return NULL;
}

bool Fortune_proto_list::Verify_prob(Fortune_head *f)
{
	int verify_prob = 0;

	unsigned int size = f->Get_Fortune_data().size();
	for (unsigned int i = 0; i < size; ++i)
		verify_prob = verify_prob + f->Get_Fortune_data()[i]->Get_fortune_data_prob();

	if (verify_prob != Fortune_head::Sum_of_prob)
		return false;

	return true;
}

int Fortune_proto_list::Make_fortune(CItem *p)
{
	if (p == NULL || p->m_itemProto == NULL)
		return Fortune_proto_list::NULL_ITEM;

	if ( !(p->m_itemProto->getItemTypeIdx() == ITYPE_WEAR &&
			p->m_itemProto->getItemSubTypeIdx() == IWEAR_SUIT) )
		return Fortune_proto_list::WRONG_TYPE;

	if (p->Aleady_got_fortune())
		return Fortune_proto_list::ALREADY_GOT_FORTUNE;

	Fortune_head *f = Get_fortune_head_proto(p->m_itemProto->getItemIndex());

	if (f == NULL)
		return Fortune_proto_list::FORTUNE_HEAD_NULL;

	Fortune_data *d = Get_fortune_result(f);

	if (d == NULL)
		return Fortune_proto_list::FORTUNE_DATA_NULL;

	p->setPlus(Make_skill_index_level_bit_sum(d));

	return Fortune_proto_list::SUCCESS;
}

int Fortune_proto_list::Make_skill_index_level_bit_sum(Fortune_data *d)
{
	if (d == NULL)
		return 0;

	int skill_index = 0, skill_level = 0;

	skill_index = d->Get_fortune_data_skill_index();
	skill_level = d->Get_fortune_data_skill_level();

	/**
	* 두 수를 하나의 변수에 담는다.
	*/
	return Make(skill_index, skill_level);
}

/**
* skill_index는 plus의 상위 2Byte, skill_level은 plus의 하위 2Byte를 사용한다.
*/
int Fortune_proto_list::Make(int skill_index, int skill_level)
{
	int plus = 0;
	plus = plus | skill_index;
	plus = plus << 16;
	plus = plus | skill_level;

	return plus;
}

/**
* 상위 2Byte를 꺼내어 skill_index에 대입하고, 하위 2Byte는 skill_level에 대입한다.
*/
void Fortune_proto_list::Resolve_fortune_value(int &skill_index, int &skill_level, int bit_sum_value)
{
	skill_index = 0;
	skill_level = 0;

	skill_level = skill_level | bit_sum_value;
	skill_level = skill_level & 0x0000FFFF;

	bit_sum_value = bit_sum_value >> 16;

	skill_index = skill_index | bit_sum_value;
}
