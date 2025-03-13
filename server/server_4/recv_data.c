#include "./include/myhead.h"

void *client_hand(void *arg)
{

    while (1)
    {
		int confd = (int)arg;
		sleep(2);
        send_all(confd, msg);
    }
}

int recv_data(int cfd, Msg *msg)
{

	sqlite3 *db;

	int i;
	int to_fd;
    int ret = 0;
    int row;
    int column;
	char sql[MAX_SIZE];
	char **result;
	char *errmsg;
	char buf[MAX_SIZE];
	char data[MAX_SIZE] = "DATA";

	memset(sql, 0, sizeof(sql));

	//打开用户数据库
	ret = sqlite3_open("resource/usr.db",&db);

    if(ret != SQLITE_OK)
    {
        printf("sqlite3 open usr.db error!\n");
        exit(1);
    }
    if(SQLITE_OK != sqlite3_get_table(db,"select * from usr",&result,&row,&column,&errmsg))
    {
        printf("select from usr error:%s\n",errmsg);
        exit(1);
    }
    for(i = 1; i <= row; i++)
    {
        to_fd = atoi(result[column * i + 3]);
    }
	//打开数据数据库
    if((sqlite3_open("resource/data.db",&db)) != SQLITE_OK)
	{
	    printf("open chatmsg.db error\n");
		exit(1);
	}


	if((sqlite3_exec(db,"create table if not exists data(data text,humidity text,temperature text,smoke text,light text);",NULL,NULL,&errmsg)) != SQLITE_OK)
	{
	    printf("create table data error:%s\n",errmsg);
		exit(1);
	}

	sprintf(sql,"insert into data(data,humidity,temperature,smoke,light) values('%s','%s','%s','%s','%s');",data,msg->msg_humidity,msg->msg_temperature,msg->msg_smoke,msg->msg_light);

	if((sqlite3_exec(db, sql, NULL, NULL,&errmsg)) != SQLITE_OK)
	{
	    printf("insert into data error:%s\n",errmsg);
		exit(1);
	}

    char arr[MAX_SIZE] = "OK";
    send(cfd, arr, sizeof(arr), 0);
	while(1)
	{
		recv(cfd, buf, sizeof(buf), 0);
		printf("Received JSON: %s\n", buf);
		// 解析JSON数据
		json_object *root = json_tokener_parse(buf);
		if (root == NULL)
		{
			printf("Failed to parse JSON.\n");
		}
		else
		{
			json_object *msg_humidity, *msg_temperature, *msg_smoke, *msg_light;
			json_object_object_get_ex(root, "humidity", &msg_humidity);
			json_object_object_get_ex(root, "temperature", &msg_temperature);
			json_object_object_get_ex(root, "smoke", &msg_smoke);
			json_object_object_get_ex(root, "light", &msg_light);
			printf("Parsed JSON:  msg_humidity=%s, msg_temperature=%s, msg_smoke=%s, msg_light=%s",
				json_object_get_string(msg_humidity),
				json_object_get_string(msg_temperature),
				json_object_get_string(msg_smoke),
				json_object_get_string(msg_light));
			strcpy(msg->msg_humidity, json_object_get_string(msg_humidity));
			strcpy(msg->msg_light, json_object_get_string(msg_light));
			strcpy(msg->msg_smoke, json_object_get_string(msg_smoke));
			strcpy(msg->msg_temperature, json_object_get_string(msg_temperature));
		}

		// sprintf(sql, "select data from data where data = '%s'",data);
    	// if(SQLITE_OK != sqlite3_get_table(db,sql,&result,&row,&column,&errmsg))
    	// {
        // 	printf("select from data.db error:%s\n",errmsg);
	    // 	exit(1);
    	// }

		// sprintf(sql, "update data set humidity = '%s' where data = '%s';", msg->msg_humidity, data);
		// if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		// {
		//     printf("update humidity error:%s\n",errmsg);
		// 	exit(1);
		// }

		// sprintf(sql, "update data set light = '%s' where data = '%s';", msg->msg_light, data);
		// if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		// {
		//     printf("update light error:%s\n",errmsg);
		// 	exit(1);
		// }

		// sprintf(sql, "update data set smoke = '%s' where data = '%s';", msg->msg_smoke, data);
		// if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		// {
		//     printf("update smoke error:%s\n",errmsg);
		// 	exit(1);
		// }

		// sprintf(sql, "update data set temperature = '%s' where data = '%s';", msg->msg_temperature, data);
		// if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		// {
		//     printf("update temperature error:%s\n",errmsg);
		// 	exit(1);
		// }

		sprintf(sql,"insert into data(data,humidity,temperature,smoke,light) values('%s','%s','%s','%s','%s');",data,msg->msg_humidity,msg->msg_temperature,msg->msg_smoke,msg->msg_light);

		if((sqlite3_exec(db, sql, NULL, NULL,&errmsg)) != SQLITE_OK)
		{
	    	printf("insert into data error:%s\n",errmsg);
			exit(1);
		}

	// 	pthread_t tid;
    // 	if (0 != pthread_create(&tid, NULL, client_hand, (void *)to_fd))
    // 	{
    //     	puts("thread create err");
    //     	close(cfd);
    //     	return -1;
    // 	}
    // 	// 线程分离
    // 	pthread_detach(tid);
	}
}
