#include "./include/myhead.h"

int changepwd(int cfd, Msg *msg)
{
    int ret = 0;
    int row;
    int column;
    
    char *errmsg;
    char **result;
    char sql[MAX_SIZE];
    char arr[MAX_SIZE] = "FORGOT";
    char buf[MAX_SIZE];

	memset(sql, 0, sizeof(sql));

    // 创建JSON对象
    json_object *root = json_object_new_object();

    sqlite3 *db;
    
    ret = sqlite3_open("resource/usr.db",&db);

    if(ret != SQLITE_OK)
    {
        printf("sqlite3 open resource/usr.db error!\n");
		exit(1);
    }
    
	sprintf(sql, "select name from usr where name = '%s'",msg->name);
    if(SQLITE_OK != sqlite3_get_table(db,sql,&result,&row,&column,&errmsg))
    {
        printf("select from usr.db error:%s\n",errmsg);
	    exit(1);
    }
    
    if(row != 1)
	{
	    // msg->action = CHANGEPWD;
		// msg->flag = FAILURE;

        strcpy(buf, "PWERR");
        json_object_object_add(root, "action", json_object_new_string(arr));
        json_object_object_add(root, "result", json_object_new_string(buf));

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
        return FAILURE;
	}
    else
	{
	    sprintf(sql, "update usr set password = '%s' where name = '%s';", msg->password, msg->name);
		if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
		    printf("update password error:%s\n",errmsg);
			exit(1);
		}
		
		// msg->action = CHANGEPWD;
	    // msg->flag = SUCCESS;
        strcpy(buf, "SUCCESS");
        json_object_object_add(root, "action", json_object_new_string(arr));
        json_object_object_add(root, "result", json_object_new_string(buf));

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
	}	

    json_object_put(root);
    sqlite3_free_table(result);
    sqlite3_close(db);
    
    return SUCCESS;
}
