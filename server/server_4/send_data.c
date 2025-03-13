#include "./include/myhead.h"

int send_all(int cfd, Msg *msg)
{
    sqlite3 *db;

	int i;
	int row;
	int column;
	
	char *errmsg;
	char **result;
	char sql[MAX_SIZE];
    char buf[MAX_SIZE] = "DATA";

    //打开数据数据库
    if((sqlite3_open("resource/data.db",&db)) != SQLITE_OK)
	{
	    printf("open chatmsg.db error\n");
		exit(1);
	}

    // memset(sql, 0, sizeof(sql));

	// if((sqlite3_exec(db,"create table if not exists data(humidity text,temperature text,smoke text,light text);",NULL,NULL,&errmsg)) != SQLITE_OK)
	// {
	//     printf("create table data error:%s\n",errmsg);
	// 	exit(1);
	// }

    // memset(sql, 0, sizeof(sql));

    memset(sql, 0, sizeof(sql));
    
	if((sqlite3_get_table(db, "select * from data", &result, &row, &column, &errmsg)) != SQLITE_OK)
	{
	    printf("select from data error:%s\n",errmsg);
		exit(1);
    }
	for(i = 1; i <= row; i++)
	{
	    bzero(msg, sizeof(Msg));
		strcpy(msg->msg_humidity,result[i*column+1]);
	    strcpy(msg->msg_temperature,result[i*column+2]);
	    strcpy(msg->msg_smoke,result[i*column+3]);
        strcpy(msg->msg_light,result[i*column+4]);
		// write(cfd, msg, sizeof(Msg));
	}

    // 创建JSON对象
    json_object *root = json_object_new_object();

    json_object_object_add(root, "action", json_object_new_string(buf));
    json_object_object_add(root, "humidity", json_object_new_string(msg->msg_humidity));
    json_object_object_add(root, "light", json_object_new_string(msg->msg_light));
    json_object_object_add(root, "smoke", json_object_new_string(msg->msg_smoke));
    json_object_object_add(root, "temperature", json_object_new_string(msg->msg_temperature));

    const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }

    json_object_put(root);
    return SUCCESS;
}

int send_smoke(int cfd, Msg *msg)
{
    char buf[MAX_SIZE] = "GRAPH_SMOKE";
    char buf_data1[MAX_SIZE] = "235";
    char buf_data2[MAX_SIZE] = "352";
    char buf_data3[MAX_SIZE] = "651";
    char buf_data4[MAX_SIZE] = "354";
    char buf_data5[MAX_SIZE] = "554";

    // 创建JSON对象
    json_object *root = json_object_new_object();

    json_object_object_add(root, "action", json_object_new_string(buf));
    json_object_object_add(root, "graph_1", json_object_new_string(buf_data1));
    json_object_object_add(root, "graph_2", json_object_new_string(buf_data2));
    json_object_object_add(root, "graph_3", json_object_new_string(buf_data3));
    json_object_object_add(root, "graph_4", json_object_new_string(buf_data4));
    json_object_object_add(root, "graph_5", json_object_new_string(buf_data5));

     const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }

    json_object_put(root);
    return SUCCESS;
}

int send_light(int cfd, Msg *msg)
{
    char buf[MAX_SIZE] = "GRAPH_LIGHT";
    char buf_data1[MAX_SIZE] = "4";
    char buf_data2[MAX_SIZE] = "56";
    char buf_data3[MAX_SIZE] = "80";
    char buf_data4[MAX_SIZE] = "12";
    char buf_data5[MAX_SIZE] = "9";

    // 创建JSON对象
    json_object *root = json_object_new_object();

    json_object_object_add(root, "action", json_object_new_string(buf));
    json_object_object_add(root, "graph_1", json_object_new_string(buf_data1));
    json_object_object_add(root, "graph_2", json_object_new_string(buf_data2));
    json_object_object_add(root, "graph_3", json_object_new_string(buf_data3));
    json_object_object_add(root, "graph_4", json_object_new_string(buf_data4));
    json_object_object_add(root, "graph_5", json_object_new_string(buf_data5));

    const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }

    json_object_put(root);
    return SUCCESS;
}

int send_temperature(int cfd, Msg *msg)
{
    char buf[MAX_SIZE] = "GRAPH_TEM";
    char buf_data1[MAX_SIZE] = "15";
    char buf_data2[MAX_SIZE] = "12";
    char buf_data3[MAX_SIZE] = "26";
    char buf_data4[MAX_SIZE] = "21";
    char buf_data5[MAX_SIZE] = "16";

    // 创建JSON对象
    json_object *root = json_object_new_object();

    json_object_object_add(root, "action", json_object_new_string(buf));
    json_object_object_add(root, "graph_1", json_object_new_string(buf_data1));
    json_object_object_add(root, "graph_2", json_object_new_string(buf_data2));
    json_object_object_add(root, "graph_3", json_object_new_string(buf_data3));
    json_object_object_add(root, "graph_4", json_object_new_string(buf_data4));
    json_object_object_add(root, "graph_5", json_object_new_string(buf_data5));

     const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }

    json_object_put(root);
    return SUCCESS;
}

int send_humidity(int cfd, Msg *msg)
{
    char buf[MAX_SIZE] = "GRAPH_HUM";
    char buf_data1[MAX_SIZE] = "45";
    char buf_data2[MAX_SIZE] = "52";
    char buf_data3[MAX_SIZE] = "43";
    char buf_data4[MAX_SIZE] = "86";
    char buf_data5[MAX_SIZE] = "25";
    // 创建JSON对象
    json_object *root = json_object_new_object();

    json_object_object_add(root, "action", json_object_new_string(buf));
    json_object_object_add(root, "graph_1", json_object_new_string(buf_data1));
    json_object_object_add(root, "graph_2", json_object_new_string(buf_data2));
    json_object_object_add(root, "graph_3", json_object_new_string(buf_data3));
    json_object_object_add(root, "graph_4", json_object_new_string(buf_data4));
    json_object_object_add(root, "graph_5", json_object_new_string(buf_data5));

     const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }

    json_object_put(root);
    return SUCCESS;
}
