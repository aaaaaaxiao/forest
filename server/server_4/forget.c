#include "./include/myhead.h"

int forget(int cfd, Msg *msg)
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

    char buf[MAX_SIZE];
    char arr[MAX_SIZE] = "FORGOT";
    char buffer[MAX_SIZE];
    char sure[MAX_SIZE] = "SURE";

    srand(time(NULL));
    msg->num = (int)rand()%1000 + 1000;

    // 创建JSON对象
    json_object *root = json_object_new_object();

    sqlite3 *db;

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
    json_object_object_add(root, "action", json_object_new_string(arr));
    // json_object_object_add(root, "result", json_object_new_string(buf));
    json_object_object_add(root, "num", json_object_new_int(msg->num));
    const char *json_string = json_object_to_json_string(root);

    // 发送JSON数据
    if (send(to_fd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
    }
    else
    {
        printf("JSON data sent: %s\n", json_string);
    }
    // printf("11\n");
    int n_read = recv(cfd, buffer, sizeof(buffer), 0);
    if (n_read == 0)
    {
        exit(1);
    }
    else
    {
        printf("Received JSON: %s\n", buffer);
    }

    // 解析JSON数据
    root = json_tokener_parse(buffer);
    if (root == NULL)
    {
        printf("Failed to parse JSON.\n");
    }
    else
    {
        json_object *num;
        json_object_object_get_ex(root, "num", &num);
        if (json_object_get_int(num) == msg->num)
        {
            // json_object_put(root);
            bzero(buffer, sizeof(buffer));
            json_object_object_add(root, "action", json_object_new_string(arr));
            json_object_object_add(root, "result", json_object_new_string(sure));
            const char *json_string = json_object_to_json_string(root);
            
            send(to_fd, json_string, strlen(json_string), 0);

            recv(cfd, buffer, sizeof(buffer), 0);
            root = json_tokener_parse(buffer);
            if (root == NULL)
            {
                printf("Failed to parse JSON.\n");
            }
            else
            {
                json_object *password;
                json_object_object_get_ex(root, "password", &password);
                strcpy(msg->password, json_object_get_string(password));
                json_object_put(root);
            }
            changepwd(to_fd,msg);
        }
    }

    // json_object_put(root);
    sqlite3_free_table(result);
    sqlite3_close(db);

    return SUCCESS;
}