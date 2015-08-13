#include <server_msgq.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	fprintf(stderr, "%d\n", sizeof(st_Character)+sizeof(st_Inven)+sizeof(st_Pet)+sizeof(st_Apets)+sizeof(st_Pet_Name)+sizeof(st_Apets_Inven)+sizeof(st_Assist_Abstime)+sizeof(st_Apets)+sizeof(st_Pet_Name)+sizeof(st_Inven)+sizeof(st_Pet)+sizeof(st_Apets)+sizeof(st_Pet_Name)+sizeof(st_Apets_Inven)+sizeof(st_Assist_Abstime)+sizeof(st_Questdata)+sizeof(st_Title)+sizeof(st_Affinity_Keep)+sizeof(st_Affinity_Reward_Step)+sizeof(st_Attendance_Exp_System));
	return 0;
}
