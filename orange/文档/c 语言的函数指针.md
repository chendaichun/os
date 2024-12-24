C语言中的函数指针

函数指针是C语言中的一种强大特性，允许通过指针调用函数。它的主要作用是实现灵活的程序设计，例如回调函数、函数数组、动态函数调用等。

1. 什么是函数指针？

函数指针是一种特殊类型的指针，它指向一个函数的起始地址。通过函数指针可以调用该函数，甚至动态决定调用哪个函数。
	•	普通指针： 指向变量的内存地址。
	•	函数指针： 指向函数的代码地址。

2. 函数指针的定义

函数指针的定义语法与普通指针类似，但需要明确函数的返回值和参数列表。

返回值类型 (*指针变量名)(参数类型列表);

例如：

int (*func_ptr)(int, float);

	•	func_ptr 是一个指向函数的指针。
	•	这个函数的返回值是 int。
	•	该函数有两个参数：一个 int，一个 float。

3. 函数指针的用法

(1) 声明和赋值

将一个函数的地址赋给函数指针。

#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int (*func_ptr)(int, int); // 声明函数指针
    func_ptr = add;            // 将函数地址赋值给指针
    printf("Result: %d\n", func_ptr(5, 3)); // 使用指针调用函数
    return 0;
}

输出：

Result: 8

(2) 函数指针作为函数参数

函数指针可以作为参数传递给另一个函数。

#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

// 使用函数指针作为参数
void compute(int x, int y, int (*operation)(int, int)) {
    printf("Result: %d\n", operation(x, y));
}

int main() {
    compute(5, 3, add);        // 传递 add 函数
    compute(5, 3, multiply);   // 传递 multiply 函数
    return 0;
}

输出：

Result: 8
Result: 15

(3) 函数指针数组

用来存储多个函数指针。

#include <stdio.h>

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

int main() {
    int (*operations[2])(int, int); // 函数指针数组
    operations[0] = add;
    operations[1] = subtract;

    printf("Addition: %d\n", operations[0](10, 5));
    printf("Subtraction: %d\n", operations[1](10, 5));
    return 0;
}

输出：

Addition: 15
Subtraction: 5

(4) 返回函数指针的函数

函数的返回值也可以是一个函数指针。

#include <stdio.h>

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

int (*get_operation(char op))(int, int) {
    if (op == '+') return add;
    if (op == '-') return subtract;
    return NULL;
}

int main() {
    int (*operation)(int, int);
    operation = get_operation('+');
    if (operation) {
        printf("Result: %d\n", operation(7, 3));
    }
    return 0;
}

输出：

Result: 10

4. 函数指针的应用场景
	1.	回调函数：
	•	函数指针作为参数传递，实现灵活的代码逻辑。
	•	常见于排序算法中，如 qsort。

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main() {
    int arr[] = {5, 2, 9, 1, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    qsort(arr, n, sizeof(int), compare); // compare 是回调函数
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}


	2.	动态函数调用：
	•	根据条件选择不同的函数，提高灵活性。
	3.	插件和模块化设计：
	•	在开发插件时，可以通过函数指针调用不同模块中的函数。
	4.	状态机设计：
	•	函数指针可以用来实现状态之间的切换逻辑。

5. 函数指针和普通指针的区别

普通指针	函数指针
指向数据的内存地址	指向代码（函数）的起始地址
操作数据	调用函数
使用 * 解引用访问数据	使用 () 调用函数

6. 注意事项
	1.	类型匹配： 函数指针的类型必须与目标函数完全一致，包括返回值和参数列表。
	2.	空指针调用： 在调用函数指针之前，确保它指向有效地址，否则会导致未定义行为。
	3.	可读性： 使用函数指针可能会降低代码的可读性，尤其在嵌套使用时，需要清晰的注释。

通过理解和使用函数指针，可以显著提升代码的灵活性和扩展性，是编写高级C程序的必备工具。