#include <stdio.h>

long long int repeats;
long long int first;
long long int last;
long long int diff;
long long int people_ingame;
int offset;

long long int getPositon_lastStanding()
{
    long long int people_out = (people_ingame + 1 - offset)/2;
    if (offset == 0)
        first += diff;
    if (people_out * 2 < people_ingame)
    {
        offset = 0;
    }
    else if (people_out * 2 == people_ingame && offset == 0)
    {
        offset = 0;
    }
    else
    {
        last -= diff;
        offset = 1;
    }
    if (first == last)
        return first;
    people_ingame -= people_out;
    diff *= 2;
    return getPositon_lastStanding();
}

long long int getPositon()
{
    long long int people_out = (people_ingame + 1 - offset)/2;
    if (repeats <= people_out)
        return ((repeats - 1) * 2 + offset) * diff + first;
    repeats -= people_out;
    if (offset == 0)
        first += diff;
    if (people_out * 2 < people_ingame)
    {
        offset = 0;
    }
    else if (people_out * 2 == people_ingame && offset == 0)
    {
        offset = 0;
    }
    else
    {
        last -= diff;
        offset = 1;
    }
    if (first == last)
        return first;
    people_ingame -= people_out;
    diff *= 2;
    return getPositon();
}

void processPositon()
{
    scanf("%lld", &people_ingame);
    if (people_ingame < 1)
        return;
    scanf("%lld", &repeats);
    if (people_ingame == 1)
    {
        printf("1\n");
        return;
    }
    if (repeats < 0)
        repeats = people_ingame + repeats + 1;
    if (repeats < 1 || repeats > people_ingame)
        return;

    first = diff = offset = 1;
    last = people_ingame;
    long long int position;
    if (repeats == people_ingame)
        position = getPositon_lastStanding();
    else
        position = getPositon();

    printf("%lld\n", position);
}

int main()
{
    long long int number_of_lines;
    scanf("%lld", &number_of_lines);
    if (number_of_lines < 1)
        return 0;

    for (int i = 0; i < number_of_lines; i++)
        processPositon();

    return 0;
}
