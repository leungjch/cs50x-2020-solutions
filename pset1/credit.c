#include <cs50.h>
#include <stdio.h>

// Return the length of a digit
long getLength(long myN)
{
    long length = 1;
    long divideSum = 1;
    for (int i = 0; myN / ((i + 1)*divideSum) != 0; i++)
    {
        // printf("n:%i\n", i);
        divideSum *= 10;
        length += 1;
    }
    return length;
}

// Get a specific digit from a number
long getDigit(long myN, int divide)
{
    long divideSum = 1;
    for (int i = 0; i < divide; i++)
    {
        divideSum *= 10;
    }
    long result = (myN / (divideSum)) % 10;
    return result;
}

long splitAdd(long n)
{
    long length = getLength(n);
    long sum = 0;
    for (int i = 0; i < length; i++)
    {
        long digit = getDigit(n, i);
        sum += digit;
    }
    return sum;
}




int main(void)
{
    bool isamex = false;
    bool isvisa = false;
    bool ismastercard = false;
    bool isinvalid = false;
    bool ischecksum = false;

    long n = get_long("Number:");
    // long n = 369421438430814;
    int length = getLength(n);



    // printf("Length: %i\n", length);

    int doubleDigitSum = 0; // take the sum of the double of every two digits
    int singleDigitSum = 0; // take the sum of the other digits

    long lastDigit = 0;
    long digit = 0;
    // moves from right to left (last digit first)
    for (int i = 0; i < length; i += 1)
    {
        digit = getDigit(n, i);
        if (i % 2 == 0)
        {
            singleDigitSum += digit;
            // printf("digit: %li got single \n", digit);

        }
        else
        {
            doubleDigitSum += splitAdd(digit * 2);
            // printf("digit: %li got double \n", digit);

        }
        // check if we are at the first digit
        if (i == length - 1)
        {
            if ((digit == 3 && lastDigit == 7) || (digit == 3 && lastDigit == 4))
            {
                isamex = true;
                isvisa = false;
                ismastercard = false;
            }
            else if ((digit == 5 && lastDigit == 1) ||
                     (digit == 5 && lastDigit == 2) ||
                     (digit == 5 && lastDigit == 3) ||
                     (digit == 5 && lastDigit == 4) ||
                     (digit == 5 && lastDigit == 5))
            {
                ismastercard = true;
                isvisa = false;
                isamex = false;
            }
            else if (digit == 4)
            {
                isvisa = true;
                isamex = false;
                ismastercard = false;
            }
            else
            {
                isvisa = false;
                isamex = false;
                ismastercard = false;
            }
        }


        // reset last digit
        lastDigit = digit;

    }
    // printf("SumDouble: %i", doubleDigitSum);
    // printf("SumSingle: %i", singleDigitSum);

    int totalSum = doubleDigitSum + singleDigitSum;
    if (totalSum % 10 == 0)
    {
        ischecksum = true;
    }
    else
    {
        ischecksum = false;
    }

    // amex is 15 length
    if (length == 15 && isamex)
    {
        isamex = true;

    }


    // visa is 13 or 16
    else if ((length == 13 && isvisa) || (length == 16 && digit == 4 && isvisa))
    {
        isvisa = true;
    }
    // mastercard is 16
    else if (length == 16 && ismastercard)
    {
        ismastercard = true;
    }
    else
    {
        ismastercard = false;
        isamex = false;
        isvisa = false;
    }


    if (ischecksum && isamex)
    {
        printf("AMEX\n");
    }
    else if (ischecksum && isvisa)
    {
        printf("VISA\n");
    }
    else if (ischecksum && ismastercard)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }


    // for (int i = 0; i < 4; i++)
    // {
    // long n = getDigit(2511,i);
    // printf("%li\n",n);
    // printf("%i\n",getLength(124));

    // }



}