/*
 * Created by Aurelien Chagnon
 */

#include "../headers/Api.h"

int main() {
    Api api("input.txt", "output.log");
    api.launch();
    return 0;
}
