#include "./include/myhead.h"

int threshold(int cfd, Msg *msg)
{
    int i;
    int to_fd;
    int ret = 0;
    int count = 0;
    int row;
    int column;

    char *errmsg;
    char **result;
    char sql[MAX_SIZE];

    // 创建JSON对象
    json_object *root = json_object_new_object();

    sqlite3 *db;

    ret = sqlite3_open("resource/online.db",&db);

    if(ret != SQLITE_OK)
    {
        printf("sqlite3 open online.db error!\n");
        exit(1);
    }
    if(SQLITE_OK != sqlite3_get_table(db,"select * from online",&result,&row,&column,&errmsg))
    {
        printf("select from online error:%s\n",errmsg);
        exit(1);
    }
    for(i = 1; i <= row; i++)
    {
        to_fd = atoi(result[column * i + 1]);
    }

    json_object_object_add(root, "humidity", json_object_new_string(msg->msg_humidity));
    json_object_object_add(root, "light", json_object_new_string(msg->msg_light));
    json_object_object_add(root, "smoke", json_object_new_string(msg->msg_smoke));
    json_object_object_add(root, "temperature", json_object_new_string(msg->msg_temperature));

    const char *json_string = json_object_to_json_string(root);
    printf("Generated JSON: %s\n", json_string);

    // 发送JSON数据
    if (send(to_fd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }
    json_object_put(root);
}
