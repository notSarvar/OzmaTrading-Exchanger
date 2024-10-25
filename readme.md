# ExchangerSimulator

## Build and Run Instructions

To build the project, follow these steps:

1. Create a build directory and navigate into it:
    ```sh
    mkdir build && cd build
    ```

2. Run CMake to configure the project and then compile it:
    ```sh
    cmake .. && make
    ```

3. Navigate to the `bin` directory and run the executable with the required arguments:
    ```sh
    cd ../bin && ./HWOzmaTrading <U> <N> <M> <min_price> <max_price>
    ```

U - user count \
N - max order size \
M - max orders count for user \
min_price, max_price - min and max price for order
***