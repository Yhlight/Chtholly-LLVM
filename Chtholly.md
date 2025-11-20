## Chtholly
Chtholly是一门基于转译C++实现的编程语言，Chtholly以简洁，高性能，编译期为特征  
Chtholly遵循零成本抽象以及运行时极简原则，尽可能把事情交给编译期进行  

Chtholly文件后缀为.cns  

### 拷贝与引用与引用
对于基本数据类型来说，例如int，double，bool，将默认使用copy进行传递  
而对于引用数据类型来说，例如string，int[]，int[4]，即字符串，数组等，将默认采用不可变引用  

对于一个引用数据类型，Chtholly往往有三种表达形式  
(arg: string)  // 不可变引用  
(arg: &string)  // 可变引用  
(arg: *string)  // 拷贝  
(arg: &&string)  // 移动  

在Chtholly中，会使用let和mut进行变量的定义  
对于不可变引用，let和mut都适用  
对于可变引用，仅且mut可用  
对于拷贝，均可用，但对于自定义对象，需要实现clone方法  
而对于移动，则均可用，但对于自定义对象，需要实现move方法  

### 注释
```Chtholly
// 单行注释

/*
多行注释
*/
```

### 程序入口
```Chtholly
fn main(args: string[]): int
{

}
```

### 变量
Chtholly使用let声明不可变变量  
使用mut声明可变变量  

Chtholly编译器会自动推导变量的类型  
Chtholly支持类型限定，但不支持类型注解，类型注解的语法对于Chtholly来说会直接改变变量的底层类型  

```Chtholly
let a = 10;
let b = 30.4;
let c = 'a';
let d = "HelloWorld";
let e = [1, 2, 3, 4];

mut a2 = 10;
```

#### 类型限定
你可以在变量名后面使用: <数据类型>来限定变量的类型  

Chtholly有如下内置数据类型  
int(默认使用的类型)  

以及精细划分的类型  
i8, i16, i32, i64  
u8, u16, u32, u64  

char  

double(默认使用的类型)  
以及精细划分的类型  
float，double，long double  

void  

bool  

string  

int[], int[4]  动态与静态数组  

class  类  

(int, int): int  函数  

```Chtholly
let a: u32 = -10;  // 错误，a的类型为u32
```

### 枚举
Chtholly使用enum创建枚举  

```
enum color
{
    red,
    green
}

let a : color = color::red;
```

### 数组
Chtholly使用类型 + []创建数组  
int[]为动态数组，int[4]为静态数组  
这两种数组的操作都是不安全的  
如果你想要安全，请使用at()方法  

at()方法最终会转译为C++的at()方法  

```Chtholly
let a: int[] = [1, 2, 3];
a[0] = 10;
a.at(0) = 10;
```

### 函数
Chtholly使用fn作为函数关键字  

```Chtholly
fn main(args: string[]): int
{

}
```
返回值类型是可选部分，可以直接省略，指出更好，和类型限定一个套路  

#### lambda函数
Chtholly支持lambda函数  
默认情况下，捕获属于不可变引用  

```Chtholly
let add = [](a: int, b: int): int { return a + b; };
```

#### 函数类型
```Chtholly
fn add(x: int, y: int): int
{
    return x + y;
}

let my_func: (int, int): int = add;
```

### class类
在Chtholly之中，使用class创建类  
不支持继承语法，可以使用组合式继承，具有public，private两种权限，默认公开  

```Chtholly
class Test
{
private:
    name: string = "yhlight";  // 可以赋予默认值
    id: int;

public:
    fn add(x, y): int
    {
        return x + y;
    }

    Test(name: string)  // 构造函数
    {
        this.name = name;
    }
}

fn main(args: string[])
{
    let test = Test();  // 空初始化
    let test3 = Test("yhlight");  // 构造函数初始化

    print(test2.name);
}
```

#### 构造函数
```Chtholly
class Test
{
private:
    name: string = "yhlight";
    id: int;

public:
    Test()
    {

    }

    Test(name: string)
    {

    }
}
```

#### 析构函数
Chtholly使用~创建析构函数  
Chtholly不是一门专注于内存安全的编程语言  
析构函数是为了让开发者在必要时管理资源  
例如局部资源释放后句柄会怎么做  

```Chtholly
class Test
{
    ~Test()
    {
        print("析构函数");
    }
}

fn test()
{
    let test = Test();
}
```

#### 对象关联
使用static创建静态函数，使用static创建静态变量  

```Chtholly
class Test
{
    static name: string = "yhlight";

    fn static test(name: string): int
    {
        
    }
}
```

#### this
this是当前对象的可变引用  
你可以使用*this进行当前对象的拷贝  
&&this为当前对象的资源转移  

### 运算符
和C++一致，即支持标准的运算符  

```nota
+ - * / %
== != > >= < <= !
& | ^ ~ << >>
= += -= *= /= %=
&& ||
++ --
```

### 命名规则
与C++一致

### 选择结构和循环结构
与C++一致，不同的是switch的case允许使用任意类型的变量作为判断依据，也能使用表达式  

```Chtholly
if (条件)
{

}
else if (条件)
{

}
else
{

}
```

```Chtholly
switch(任意类型的变量 / 表达式)
{
    case 值1: {  // C语言缺陷，现代化编程语言最好强制要求{ }
        break;  // break现在不是防止穿透，而是跳出匹配
    }
    case 表达式: {
        break;
    }
    case 表达式2: {
        fallthrough;  // 如果需要穿透，请使用fallthrough
    }
}
```

```Chtholly
while(表达式)
{

}

for(let i = 0; i < 10; i++)
{

}

for(let i : 容器)
{

}

do {

} while(表达式);
```

### 泛型
#### 泛型函数
```Chtholly
fn add<T>(a: T, b: T): T
{
    return a + b;
}

add<int>(1, 2);

fn add<T = double>(a: T, b: T): T  // 默认值
{
    return a + b;
}

fn add<string>(a: string, b: string): string  // 特例化
{
    return concat(a, b);
}
```

#### 泛型类
```Chtholly
class Point<T>
{
    x: T,
    y: T,

    fn swap(other: &Point<T>)
    {
        
    }

    fn swap(other: &Point<T = int>)
    {

    }

    fn swap(other: &Point<int>)
    {

    }
}

class Point<T = int>
{

}

class Point<int>
{

}

let p = Point<int>(1, 2);
```

#### 类内的泛型函数
在Chtholly之中，类，无论自身是否是泛型，都可以拥有泛型成员函数。

##### 常规类内的泛型函数
```Chtholly
class Printer
{
    fn print<T>(value: T)
    {
        print(value);
    }
}

fn main()
{
    let p = Printer{};
    p.print<int>(10);  // 调用时指定类型
    p.print("hello");  // 或者让编译器自动推断类型
}
```

##### 泛型类内的泛型函数
泛型类内部也可以创建独立的泛型函数。
```Chtholly
class Point<T>
{
  // 方法的泛型参数 K, F 与类的泛型参数 T 是独立的
    fn test<K, F>()
    {

    }

    fn test2<K = int, F>()
    {

    }

    fn test2<K, F = int>()
    {

    }

    fn test2<K = int, F = int>()
    {

    }

    fn test2<int, int>()
    {

    }

}
```

## 类型转换
Chtholly支持类型转换，使用type_cast<T>()进行安全转换  
此函数为内置函数，不需要导入  

```chtholly
let a: i8 = (i8)(10.5);  // 不安全的转换，强制截断
let a: i8 = type_cast<int8>(10.5);  // 安全转换
```

### 模块与 `import`
Chtholly 支持模块系统，允许您将代码组织到多个文件中或使用标准库功能，`import`关键字用于加载另一个模块中的代码并将其合并到当前作用域  

#### 语法
`import` 语句接受两种形式：  
1.  **文件路径**: 一个字符串字面量，表示您想要包含的 Chtholly 文件（`.cns`）的路径  
2.  **标准库模块名**: 一个标识符，表示您想要导入的标准库模块  

```Chtholly
// 导入文件模块
import "path/to/your_module.cns";

// 导入标准库模块
import iostream;
```

#### 行为
你需要使用模块名::方法名来访问模块中的方法  

```Chtholly
import iostream;
import iostream::println;  // 把这个函数导入到当前作用域

fn main()
{
    iostream::print("Hello, World!");
    println("Hello, World!");
}
```

#### 示例
假设您有一个名为 `math.cns` 的文件：

```Chtholly
// math.cns
fn add(a: int, b: int): int
{
    return a + b;
}
```

您可以在另一个文件 `main.cns` 中通过导入 `math.cns` 来使用 `add` 函数：

```Chtholly
// main.cns
import "math.cns";

fn main()
{
    let result = math::add(5, 10);
    print(result); // 将输出 15
}
```

#### 冲突防止
通常情况下，Chtholly默认使用文件名作为模块名  
在冲突时，你可以为模块指定一个别名  

```Chtholly
import "math.cns" as math;
import "math.cns" as math2;
```

### 创建模块的最佳实践
我们建议模块使用对象式而不是函数式  

### 包
模块被收录在哪一个包？  

```nota
// 例如这个是数学模块
package packageName;

fn add(a: int, b: int): int
{
    return a + b;
}

/////////////////////
import packageName::math;
import packageName2::math;

packageName::math::add(1, 2);  // 冲突时需要写出包名
```
