#include <stdio.h>

#define COST_PER_CAR 20
#define ITEM_FREE_WEIGHT 100
#define COST_PER_ITEM 5
#define MAX_ITEMS 15
#define MAX_VAULE 1000

struct Item
{
    int profit;
    int weight;
};

Item buffer[MAX_ITEMS];
int number_of_items;
Item* items;

int calculateProfit(const int profit, const int capacity_left, const int index)
{
    if (index >= number_of_items)
        return profit;

    int profit_1 = calculateProfit(profit, capacity_left, index+1);
    int new_capacity_left = capacity_left-items[index].weight;
    if (new_capacity_left < 0)
        return profit_1;

    int profit_2 = calculateProfit(profit+items[index].profit, new_capacity_left, index+1);

    if (profit_1 > profit_2)
        return profit_1;

    return profit_2;
}

int calculateProfitBack(const int profit, const int overweight, const int index)
{
    if (overweight <= 0)
        return profit;

    if (index >= number_of_items)
        return 0;

    int profit_1 = calculateProfitBack(profit, overweight, index+1);
    int profit_2 = calculateProfitBack(profit-items[index].profit, overweight-items[index].weight, index+1);

    if (profit_1 > profit_2)
        return profit_1;

    return profit_2;
}

int main()
{
    int number_of_cars;
    if (!scanf("%i", &number_of_cars))
        return 0;
    if (number_of_cars < 1)
        return 0;

    for (int i = 0; i < number_of_cars; i++)
    {
        if (!scanf("%i", &number_of_items))
            return 0;
        if (number_of_items < 1 || number_of_items > MAX_ITEMS)
            return 0;

        for (int j = 0; j < number_of_items; j++)
        {
            if (!scanf("%i", &buffer[j].profit))
                return 0;
            if (buffer[j].profit < 1 || buffer[j].profit > MAX_VAULE)
                return 0;
        }

        for (int j = 0; j < number_of_items; j++)
        {
            if (!scanf("%i", &buffer[j].weight))
                return 0;
            if (buffer[j].weight < 1 || buffer[j].weight > MAX_VAULE)
                return 0;

            if (buffer[j].weight > ITEM_FREE_WEIGHT)
                buffer[j].profit -= COST_PER_ITEM;
        }
        int max_weight;
        if (!scanf("%i", &max_weight))
            return 0;

        int count_passed = 0;
        for (int j = 0; j < number_of_items; j++)
        {
            if (buffer[j].weight <= max_weight && buffer[j].profit >  0)
                count_passed++;
        }
        items = new Item[count_passed];
        int k = 0;
        int whole_profit = 0;
        int whole_weight = 0;
        for (int j = 0; j < number_of_items; j++)
        {
            if (buffer[j].weight <= max_weight && buffer[j].profit >  0)
            {
                items[k] = buffer[j];
                k++;
                whole_profit += buffer[j].profit;
                whole_weight += buffer[j].weight;
            }
        }
        number_of_items = count_passed;

        int profit = calculateProfitBack(whole_profit, whole_weight-max_weight, 0);

        profit -= COST_PER_CAR;
        if (profit < 0)
        {
            printf("%d\n", 0);
        }
        else
        {
            printf("%d\n", profit);
        }

        delete[] items;
    }

    return 0;
}
