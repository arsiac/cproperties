# cproperties
读写properties配置文件

### usage

- create

  ``` c
  cProperties *properties = newProperties();

  Properties cproperties;
  ```

  - create from a configuration file

    ``` c
    cProperties *properties = createPropertiesFromFile("filePath");

    Properties cproperties("filePath");
    // or
    Properties cproperties;
    cproperties.load("filePath");
    ```

- contains

  ``` c
  int result = cPropertiesContainsKey(properties, "key");
  ```

- get

  ``` c
  const char *value = cPropertiesGet(properties, "key");

  char* value = cproperties.get("key");
  ```

- add/alter

  ``` c
  int success = cPropertiesSet(properties, "key", "value");

  cproperties.set("key", "value");
  ```

- delete

  ``` c
  int success = cPropertiesDelete(properties, "key");

  cproperties.remove("key);
  ```

- save

  ``` c
  int success = cPropertiesWriteToFile(properties, "filePath");

  cproperties.save("path");
  ```



### example

- example 1

``` properties
# Zhang San
user.name=ZhangSan
  user.age=22
 user.password =123456
   user.phone    =    1234567890 #annotation
  # annotation
  syntax error 1
    syntax error 2
 user.height=170cm
```

- result 1
``` properties
user.name=ZhangSan
user.password=123456
user.phone=1234567890
user.height=170cm
```