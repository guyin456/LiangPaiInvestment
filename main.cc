#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// Define a struct to store a trade
struct Trade {
    long long timestamp;
    string stock;
    int quantity;
    int price;
};

// Define a struct to store the metrics for a stock
struct StockMetrics {
    long long last_timestamp;
    long long max_time_gap;
    int total_volume;
    long long weighted_sum;
    int total_quantity;
    int max_trade_price;

    StockMetrics() {
        last_timestamp = -1;
        max_time_gap = 0;
        total_volume = 0;
        weighted_sum = 0;
        total_quantity = 0;
        max_trade_price = 0;
    }

    void update(const Trade& trade) {
        if (last_timestamp != -1) {
            long long time_gap = trade.timestamp - last_timestamp;
            if (time_gap > max_time_gap) {
                max_time_gap = time_gap;
            }
        }
        last_timestamp = trade.timestamp;
        total_volume += trade.quantity;
        weighted_sum += static_cast<long long>(trade.quantity) * trade.price;
        total_quantity += trade.quantity;
        if (trade.price > max_trade_price) {
            max_trade_price = trade.price;
        }
    }

    int weighted_average_price() const {
        if (total_quantity == 0) {
            return 0;
        } else {
            return static_cast<int>(weighted_sum / total_quantity);
        }
    }
};

// Function to read the input file and update stock metrics
void update_stock_metrics(const string& filename, map<string, StockMetrics>& stock_metrics) {
    ifstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return;
    }

    string line;
    // Read the file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string timestamp_str, stock, quantity_str, price_str;

        // Get elements from line
        getline(ss, timestamp_str, ',');
        getline(ss, stock, ',');
        getline(ss, quantity_str, ',');
        getline(ss, price_str, ',');

        // Convert to the corresponding types
        long long timestamp = stoll(timestamp_str);
        int quantity = stoi(quantity_str);
        int price = stoi(price_str);
        Trade trade = {timestamp, stock, quantity, price};

        // Update the metrics for the stock
        auto& metrics = stock_metrics[trade.stock];
        metrics.update(trade);
    }

    file.close();
}

// function to sort stock and write ti output.csv
void write_metrics_to_file(const map<string, StockMetrics>& stock_metrics, const string& output_file_name) {
    // Write the metrics to file
    ofstream output_file(output_file_name);
    if (!output_file.is_open()) {
        cout << "Failed to open output file." << endl;
        return;
    }

    for (const auto& p : stock_metrics) {
        const string& stock = p.first;
        const StockMetrics& metrics = p.second;

        output_file << stock << ",";
        output_file << metrics.max_time_gap << ",";
        output_file << metrics.total_volume << ",";
        output_file << metrics.weighted_average_price() << ",";
        output_file << metrics.max_trade_price << endl;
    }

    output_file.close();
}


int main() {
    map<string, StockMetrics> stock_metrics;
    const string input_file_name = "input.csv";
    const string output_file_name = "output.csv";

    // Read the input file and update stock metrics
    update_stock_metrics(input_file_name, stock_metrics);

    // Write stock metrics to output file
    write_metrics_to_file(stock_metrics, output_file_name);

    return 0;
}
