#include "./include/myhead.h"

int login(int cfd, Msg *msg)
{
    int i;
    int count = 0;
    int ret = 0;
    int row;
    int column;
    char *errmsg;
    char **result;
    char buf[MAX_SIZE];
    char arr[MAX_SIZE] = "LOGIN";

    sqlite3 *db;

    // 创建JSON对象
    json_object *root = json_object_new_object();

    ret = sqlite3_open("resource/usr.db", &db); // 打开一个存储用户信息的库

    if (ret != SQLITE_OK)
    {
        printf("sqlite3 open error!\n");
        exit(1);
    }

    if (SQLITE_OK != sqlite3_get_table(db, "select * from usr", &result, &row, &column, &errmsg))
    {
        printf("select from usr error:%s\n", errmsg);
        exit(1);
    }

    if (msg->flag == 0) // 用户名登录
    {
        for (i = 1; i <= row; i++)
        {
            if (strcmp(result[column * i], msg->name) == 0)
            {
                if (strcmp(result[column * i + 2], msg->password) == 0)
                {
                    msg->action = SUCCESS; // 成功
                    strcpy(buf, "SUCCESS");
                }
                else
                {
                    msg->action = PWERR; // 失败
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
                    return PWERR;
                }
            }
            else
            {
                count++;
            }
        }
        if (count == row)
        {
            msg->action = NOUSR;
            strcpy(buf, "NOUSR");
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
            return NOUSR; // 没有用户
        }
    }
    else if (msg->flag == 1) // ID登录
    {
        for (i = 1; i <= row; i++)
        {
            if (atoi(result[column * i + 1]) == msg->id)
            {
                if (strcmp(result[column * i + 2], msg->password) == 0)
                {
                    strcpy(msg->name, result[column * i]);
                    msg->action = SUCCESS;
                    strcpy(buf, "SUCCESS");
                }
                else
                {
                    msg->action = PWERR;
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
                    return PWERR;
                }
            }
            else
            {
                count++;
            }
        }
        if (count == row)
        {
            msg->action = NOUSR;
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
            return NOUSR;
        }
    }
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
    
    json_object_put(root);
    sqlite3_free_table(result);
    sqlite3_close(db);

    return SUCCESS;
}
