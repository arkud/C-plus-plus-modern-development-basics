int Factorial(int arg_x) {
    if (arg_x <= 0)
        return 1;

    int result = 1;
    for (int i = 1; i <= arg_x; i++)
        result *= i;

    return result;
}
