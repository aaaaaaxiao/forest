#include "./include/myhead.h"

int reg(int cfd, Msg *msg)
{
    int i;
    int ret = 0;
    int row;
    int column;
    unsigned long id;
    char arr[MAX_SIZE] = "REG";

    char *errmsg;
    char **result;
    char sql[MAX_SIZE];
    char buf[MAX_SIZE];

    srand(time(NULL));
    id = (unsigned long)rand()%100000000 + 100000000;

    sqlite3 *db;

    // 创建JSON对象
	json_object *root = json_object_new_object();

    ret = sqlite3_open("resource/usr.db",&db);

    if(ret != SQLITE_OK)
    {
        printf("sqlite3 open error!\n");
        exit(1);
    }

    if(SQLITE_OK != sqlite3_exec(db,"create table if not exists usr(name text primary key,id integer,password text,cfd integer);",
                NULL,NULL,&errmsg))
    {
        printf("create table usr error:%s\n",errmsg);
        exit(1);
    }

    if(SQLITE_OK != sqlite3_get_table(db,"select * from usr",&result,&row,&column,&errmsg))
    {
        printf("select from usr error:%s\n",errmsg);
        exit(1);
    }

    for(i = 1; i <= row; i++)
    {
        if(strcmp(result[column * i], msg -> name) == 0)
        {
            msg->action = FAILURE;
            strcpy(buf, "FAILURE");

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
    }	

    sprintf(sql,"insert into usr(name, id, password, cfd) values('%s',%ld,'%s','%d');",msg->name,id,msg->password,cfd);

    if(SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
    {
        printf("insert into usr error:%s\n",errmsg);
        exit(1);
    }

    // bzero(msg, sizeof(Msg));
    // msg->action = SUCCESS;
    // msg->id = id;
    // write(cfd, msg, sizeof(Msg));
    strcpy(buf, "SUCCESS");

    // for(int i = 9; i >= 0; i--){
    //     msg->id[i] = id % 10;
    //     id /= 10;
    // }
    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", msg->id[i]);
    // }

    json_object_object_add(root, "action", json_object_new_string(arr));
    json_object_object_add(root, "id", json_object_new_int64(id));
    json_object_object_add(root, "result", json_object_new_string(buf));

	const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

	// 发送JSON数据
    if (send(cfd, json_string, strlen(json_string), 0) < 0) {
        perror("Send failed");
    } else {
        printf("JSON data sent: %s\n", json_string);
    }
    json_object_put(root);
    sqlite3_free_table(result);
    sqlite3_close(db);

    return SUCCESS;
}
