#include "./include/myhead.h"

// // 生成 +IPD 格式的消息
// void generate_ipd_message(char *buffer, const char *data)
// {
// 	int data_len = strlen(data);
// 	snprintf(buffer, MAX_SIZE, "+IPD,%d:%s", data_len, data);
// }

void *pthread_read(void *arg)
{
	int i;
	int ret = 0;
	int row;
	int column;

	char *errmsg;
	char **result;
	char sql[MAX_SIZE];

	int cfd = *((int *)arg);
	int n_read;
	// Msg *msg;
	char buffer[MAX_SIZE] = {0};
	char buf[MAX_SIZE] = {0};
	char arr[MAX_SIZE] = "OK";
	// char arr[MAX_SIZE] = {0};
	// const char *message_to_send = "OK";
	// if((msg = (Msg*)malloc(sizeof(Msg))) == NULL)
	// {
	//     perror("msg malloc:");
	//     exit(1);
	// }

	// // 生成 +IPD 格式的消息
	// generate_ipd_message(arr, message_to_send);

	// // 发送数据
	// if (send(cfd, arr, strlen(arr), 0) != strlen(arr)) {
	//     perror("send failed");
	// } else {
	//     printf("数据发送成功: %s\n", arr);
	// }

	read(cfd, buf, sizeof(buf));
	printf("Received JSON: %s\n", buf);
	// 解析JSON数据
	json_object *root = json_tokener_parse(buf);
	if (root == NULL)
	{
		printf("Failed to parse JSON.\n");
	}
	else
	{
		json_object *type;
		json_object_object_get_ex(root, "type", &type);
		printf("Parsed JSON: type=%s\n", json_object_get_string(type));
		strcpy(msg->n_action, json_object_get_string(type));
	}

	sqlite3 *db;

	ret = sqlite3_open("resource/online.db", &db);

	if (ret != SQLITE_OK)
	{
		printf("sqlite3 open error!\n");
		exit(1);
	}

	if (SQLITE_OK != sqlite3_exec(db, "create table if not exists online(QT integer,ST integer);",
								  NULL, NULL, &errmsg))
	{
		printf("create table online error:%s\n", errmsg);
		exit(1);
	}

	if (SQLITE_OK != sqlite3_get_table(db, "select * from online", &result, &row, &column, &errmsg))
	{
		printf("select from online error:%s\n", errmsg);
		exit(1);
	}

	if (strcmp(msg->n_action, "QT") == 0)
	{
		sprintf(sql, "insert into online(QT) values('%d');", cfd);
	}
	else if (strcmp(msg->n_action, "ST") == 0)
	{
		sprintf(sql, "insert into online(ST) values('%d');", cfd);
		// 		// 生成 +IPD 格式的消息
		// generate_ipd_message(arr, message_to_send);

		// // 发送数据
		// if (send(cfd, arr, strlen(arr), 0) != strlen(arr)) {
		//     perror("send failed");
		// } else {
		//     printf("数据发送成功: %s\n", arr);
		// }
		send(cfd, "OK", 3, 0);
		puts(arr);
	}

	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("insert into online error:%s\n", errmsg);
		exit(1);
	}

	while (1)
	{
		bzero(msg, sizeof(Msg));
		// 接收JSON数据
		n_read = recv(cfd, buffer, sizeof(buffer), 0);
		if (n_read == 0)
		{
			perror("client close!\n");
			break;
		}
		else
		{
			printf("Received JSON: %s\n", buffer);
			// 解析JSON数据
			json_object *root = json_tokener_parse(buffer);
			if (root == NULL)
			{
				printf("Failed to parse JSON.\n");
			}
			else
			{
				json_object *flag, *action, *name, *id, *password, *msg_humidity, *msg_temperature, *msg_smoke, *msg_light;
				json_object_object_get_ex(root, "flag", &flag);
				json_object_object_get_ex(root, "action", &action);
				json_object_object_get_ex(root, "name", &name);
				json_object_object_get_ex(root, "id", &id);
				json_object_object_get_ex(root, "password", &password);
				json_object_object_get_ex(root, "humidity", &msg_humidity);
				json_object_object_get_ex(root, "temperature", &msg_temperature);
				json_object_object_get_ex(root, "smoke", &msg_smoke);
				json_object_object_get_ex(root, "light", &msg_light);
				printf("Parsed JSON: flag=%d, action=%s, name=%s, id=%s, password=%s, msg_humidity=%s, msg_temperature=%s, msg_smoke=%s, msg_light=%s",
					   json_object_get_int(flag),
					   json_object_get_string(action),
					   json_object_get_string(name),
					   json_object_get_string(id),
					   json_object_get_string(password),
					   json_object_get_string(msg_humidity),
					   json_object_get_string(msg_temperature),
					   json_object_get_string(msg_smoke),
					   json_object_get_string(msg_light));
				strcpy(msg->n_action, json_object_get_string(action));

				if (strcmp(msg->n_action, "REG") == 0)
				{
					strcpy(msg->name, json_object_get_string(name));
					strcpy(msg->password, json_object_get_string(password));
					msg->action = REG;
				}
				else if (strcmp(msg->n_action, "LOGIN") == 0)
				{
					msg->flag = json_object_get_int(flag);
					if (msg->flag == 0)
					{
						strcpy(msg->name, json_object_get_string(name));
						strcpy(msg->password, json_object_get_string(password));
					}
					else
					{
						strcpy(msg->n_id, json_object_get_string(id));
						strcpy(msg->password, json_object_get_string(password));
						msg->id = strtoull(msg->n_id, NULL, 10);
						// printf("ID = %s\n",msg->n_id);
						// printf("ID = %ld\n",msg->id);
					}
					msg->action = LOGIN;
				}
				else if (strcmp(msg->n_action, "FORGOT") == 0)
				{
					strcpy(msg->name, json_object_get_string(name));
					msg->action = FORGOT;
				}
				else if (strcmp(msg->n_action, "MONITOR") == 0)
				{
					msg->action = MONITOR;
				}
				else if (strcmp(msg->n_action, "THRESHOLD") == 0)
				{
					strcpy(msg->msg_humidity, json_object_get_string(msg_humidity));
					strcpy(msg->msg_light, json_object_get_string(msg_light));
					strcpy(msg->msg_smoke, json_object_get_string(msg_smoke));
					strcpy(msg->msg_temperature, json_object_get_string(msg_temperature));
					msg->action = THRESHOLD;
				}
				else if (strcmp(msg->n_action, "DATA") == 0)
				{
					msg->action = DATA;
				}
				else if (strcmp(msg->n_action, "SMOKE") == 0)
				{
					msg->action = SMOKE;
				}
				else if (strcmp(msg->n_action, "HUMIDITY") == 0)
				{
					msg->action = HUMIDITY;
				}
				else if (strcmp(msg->n_action, "TEMPERATURE") == 0)
				{
					msg->action = TEMPERATURE;
				}
				else if (strcmp(msg->n_action, "LIGHT") == 0)
				{
					msg->action = LIGHT;
				}
				else if (strcmp(msg->n_action, "RECV") == 0)
				{
					msg->action = RECV;
				}
				else if (strcmp(msg->n_action, "BEEP") == 0)
				{
					msg->action = BEEP;
				}
				json_object_put(root);
			}
		}

		switch (msg->action)
		{
			case REG: // 注册
			{
				reg(cfd, msg);
				break;
			}
			case LOGIN: // 登陆
			{
				if (login(cfd, msg) == SUCCESS)
				{
					printf("online successfully!\n");
				}
				break;
			}
			case FORGOT: // 忘记密码
			{
				forget(cfd, msg);
				break;
			}
			case MONITOR: // 摄像头监控
			{
				camera(cfd, msg);
				sleep(1);
				break;
			}
			case THRESHOLD: // 报警
			{
				threshold(cfd, msg);
				break;
			}
			case DATA: // 发送数据
			{
				send_all(cfd, msg);
				break;
			}
			case RECV:
			{
				recv_data(cfd, msg);
				break;
			}
			case SMOKE:
			{
				send_smoke(cfd, msg);
				break;
			}
			case HUMIDITY:
			{
				send_humidity(cfd, msg);
				break;
			}
			case TEMPERATURE:
			{
				send_temperature(cfd, msg);
				break;
			}
			case LIGHT:
			{
				send_light(cfd, msg);
				break;
			}
			case BEEP:
			{
				send_beep(cfd, msg);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return NULL;
}
