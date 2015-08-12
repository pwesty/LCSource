#ifndef __FORTUNE_SYSTEM_H__
#define __FORTUNE_SYSTEM_H__
#include "../ShareLib/DBCmd.h"
#include "Log.h"
#include "Item.h"

class Fortune_data
{
public:
	Fortune_data(int _skill_index, int _skill_level, int _prob);
	virtual ~Fortune_data();
	int Get_fortune_data_prob();
	int Get_fortune_data_skill_index();
	int Get_fortune_data_skill_level();

private:
	int Skill_index;
	int Skill_level;
	int Prob;
};

class Fortune_head
{
public:
	enum PROB_TYPE
	{ PROB, RANDOM };

	const static int Sum_of_prob = 10000;

public:
	Fortune_head(int _item_index, int _prob_type);
	virtual ~Fortune_head();
	std::vector<Fortune_data*> &Get_Fortune_data();
	int Get_item_index();
	int Get_prob_type();

private:
	void Fortune_data_clear();
	void Delete_fortune_data_vec();

private:
	int Item_index;
	int Prob_type;
	std::vector<Fortune_data*> fd;
};

class Fortune_proto_list
{
public:
	Fortune_proto_list();
	virtual ~Fortune_proto_list();
public:
	enum ERROR_TYPE
	{ SUCCESS, NULL_ITEM, WRONG_TYPE, FORTUNE_HEAD_NULL, FORTUNE_DATA_NULL, ALREADY_GOT_FORTUNE };

	bool Make_fortune_proto_list(MYSQL *dbdata);
	Fortune_head *Get_fortune_head_proto(int Item_index);
	Fortune_data *Get_fortune_result(Fortune_head *f);
	int Make_fortune(CItem *p);
	void Resolve_fortune_value(int &skill_index, int &skill_level, int bit_sum_value);

private:
	int Make_skill_index_level_bit_sum(Fortune_data *d);
	int Make(int skill_index, int skill_level);
	bool Verify_prob(Fortune_head *f);

private:
	std::vector<Fortune_head*> fh;
};

class Compare_fortune_list : public std::binary_function<Fortune_head*, Fortune_head*, bool>
{
public:
	bool operator()(Fortune_head* l, Fortune_head* r) const
	{
		return ( l->Get_item_index() == r->Get_item_index() );
	}
};
#endif // __FORTUNE_SYSTEM_H__
//
