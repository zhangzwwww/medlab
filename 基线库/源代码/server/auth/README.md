# AUTH

## 部署

前置条件：

* docker

部署方法：

安装 mysql 

```bash
docker pull mysql
docker run --name mysql -p yourMySQLPort:3306 -e MYSQL\_ROOT\_PASSWORD=yourMysqlPassowrd -d mysql
```

创建表

```bash
mysql -hyourMySQLHost -PyourMySQLPort -uroot -p -Dauth<./migration.sql
```

安装 redis

```bash
docker pull redis:latest
docker run --name redis -d -p yourRedisPort:6379 redis
```

将文件夹中的 Dockerfile.swap 更改为 Dockerfile 将里面的环境变量填写为之前安装时的内容。

创建服务

```bash
docker build -t 'auth' .
docker run -d -p 5000:5000 --name auth auth
```

