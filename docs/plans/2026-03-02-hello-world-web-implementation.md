# Hello World Web 应用实现计划

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** 使用 Drogon 框架创建一个 C++ web 服务器，访问主页时返回 "hello world"。

**Architecture:** 基于 Drogon 框架的 RESTful API 服务器，使用 MVC 模式，通过控制器处理 HTTP 请求。

**Tech Stack:** C++20, CMake, Drogon Web Framework, Ninja

---

### Task 1: 安装 Drogon 依赖

**Files:**
- None (system packages)

**Step 1: 安装 Drogon 依赖包**

```bash
sudo apt update
sudo apt install -y cmake build-essential libssl-dev zlib1g-dev uuid-dev libpcre2-dev
```

**Step 2: 克隆并编译 Drogon**

```bash
cd /tmp
git clone https://github.com/drogonframework/drogon.git
cd drogon
git submodule update --init
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
sudo cmake --install .
```

**Step 3: 验证 Drogon 安装**

```bash
drogon_ctl --version
```

Expected: Drogon control tool version output

**Step 4: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add docs/plans/2026-03-02-hello-world-web-implementation.md
git commit -m "docs: add implementation plan for hello world web app"
```

---

### Task 2: 更新 CMakeLists.txt

**Files:**
- Modify: `CMakeLists.txt`

**Step 1: 写入失败的测试（先运行现有项目验证）**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
ninja clean
```

Expected: Build directory cleaned

**Step 2: 更新 CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.14)
project(hello)

set(CMAKE_CXX_STANDARD 20)

find_package(Drogon CONFIG REQUIRED)

add_executable(hello main.cpp)

target_link_libraries(hello PRIVATE Drogon::Drogon)
```

**Step 3: 运行 CMake 重新配置**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
cmake ..
```

Expected: CMake configuration succeeds, finds Drogon

**Step 4: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add CMakeLists.txt
git commit -m "build: update CMakeLists.txt for Drogon framework"
```

---

### Task 3: 创建 HelloController

**Files:**
- Create: `HelloController.h`
- Create: `HelloController.cc`

**Step 1: 创建 HelloController.h**

```cpp
#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
class HelloController : public drogon::HttpController<HelloController> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(HelloController::hello, Get, "/")
    METHOD_LIST_END

    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);
};
} // namespace api
```

**Step 2: 创建 HelloController.cc**

```cpp
#include "HelloController.h"

using namespace drogon;

namespace api {

void HelloController::hello(const HttpRequestPtr &req,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newHttpViewResponse("hello world");
    callback(resp);
}

} // namespace api
```

**Step 3: 编译检查（预期失败）**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
ninja
```

Expected: FAIL - main.cpp doesn't use the controller yet

**Step 4: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add HelloController.h HelloController.cc
git commit -m "feat: add HelloController with hello endpoint"
```

---

### Task 4: 更新 main.cpp 启动 Drogon 服务器

**Files:**
- Modify: `main.cpp`

**Step 1: 备份原 main.cpp**

```bash
cp main.cpp main.cpp.bak
```

**Step 2: 更新 main.cpp**

```cpp
#include <drogon/drogon.h>
#include "HelloController.h"

int main() {
    // 设置日志级别
    drogon::app().setLogPath("./")
                .setLogLevel(trantor::Logger::kWarn);

    // 加载控制器
    drogon::app().registerController(
        std::make_shared<api::HelloController>()
    );

    // 监听端口 8080
    drogon::app().addListener("0.0.0.0", 8080);

    // 设置线程数
    drogon::app().setThreadNum(16);

    // 启动服务器
    std::cout << "Server starting on http://0.0.0.0:8080" << std::endl;
    drogon::app().run();

    return 0;
}
```

**Step 3: 编译并验证**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
ninja
```

Expected: SUCCESS - binary built successfully

**Step 4: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add main.cpp main.cpp.bak
git commit -m "feat: add Drogon server entry point"
```

---

### Task 5: 启动服务器并测试

**Files:**
- None

**Step 1: 启动服务器**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
./hello &
```

Expected: "Server starting on http://0.0.0.0:8080"

**Step 2: 测试 HTTP 请求**

```bash
curl http://localhost:8080/
```

Expected: `hello world`

**Step 3: 测试失败情况（返回 404）**

```bash
curl http://localhost:8080/notfound
```

Expected: 404 Not Found

**Step 4: 停止服务器**

```bash
pkill hello
```

Expected: Server stopped

**Step 5: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add -A
git commit -m "test: verify hello world endpoint works"
```

---

### Task 6: 添加健康检查端点（可选增强）

**Files:**
- Modify: `HelloController.h`
- Modify: `HelloController.cc`

**Step 1: 更新 HelloController.h 添加健康检查路由**

```cpp
#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
class HelloController : public drogon::HttpController<HelloController> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(HelloController::hello, Get, "/")
    METHOD_ADD(HelloController::health, Get, "/health")
    METHOD_LIST_END

    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);

    void health(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
};
} // namespace api
```

**Step 2: 更新 HelloController.cc 实现健康检查**

```cpp
#include "HelloController.h"
#include <drogon/utils/Utilities.h>

using namespace drogon;

namespace api {

void HelloController::hello(const HttpRequestPtr &req,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newHttpViewResponse("hello world");
    callback(resp);
}

void HelloController::health(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value json;
    json["status"] = "ok";
    json["timestamp"] = utils::getHttpFullDate();
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

} // namespace api
```

**Step 3: 重新编译**

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
ninja
```

Expected: SUCCESS

**Step 4: 启动并测试健康检查**

```bash
./hello &
sleep 1
curl http://localhost:8080/health
pkill hello
```

Expected: `{"status":"ok","timestamp":"..."}`

**Step 5: 提交**

```bash
cd /home/ubuntu/workspace/hello
git add HelloController.h HelloController.cc
git commit -m "feat: add health check endpoint"
```

---

## 验证步骤

完成所有任务后，运行以下命令验证：

```bash
cd /home/ubuntu/workspace/hello/cmake-build-debug
./hello &
sleep 1
curl http://localhost:8080/
curl http://localhost:8080/health
pkill hello
```

预期输出：
```
hello world
{"status":"ok","timestamp":"..."}
```
