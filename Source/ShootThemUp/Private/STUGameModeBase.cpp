// Shoot Them Up Game. All Rights Reserved

#include "STUGameModeBase.h"

#include <iostream>
using namespace std;

struct A
{
    inline static const int f() { return 9; }
};

int main()
{
    int n;
    cout << "Enter a number:";
    cin >> n;
    if (n % 2 == 0)
    {
        cout << "Even number";
    }
    else
    {
        cout << "Odd number";
    }
    for (int i = 0; i < 5; i++)
    {
        cout << "Loop iteration: " << i << endl;
    }
    int arr[] = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int j = 0; j < 5; j++)
    {
        sum += arr[j];
    }
    cout << "Sum of array elements: " << sum << endl;
    return 0;
}