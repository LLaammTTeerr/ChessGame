#include "StockfishHandler.h"
#include "chess.hpp"
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <qdebug.h>
#include <QMessageBox>

namespace bp = boost::process;
namespace asio = boost::asio;

StockFish::StockFish(int depth = 20) {
	this->depth = depth;
    asio::io_context io;

    bp::ipstream stockfish_stdout;
    bp::opstream stockfish_stdin;

    // Replace with the correct path to Stockfish executable
#ifdef _WIN32
    std::string stockfish_path = "./resources/files/stockfish.exe";
#else
    std::string stockfish_path = "./stockfish";
#endif

    bp::child stockfish_process(
        stockfish_path,
        bp::std_out > stockfish_stdout,
        bp::std_in < stockfish_stdin
    );

    std::mutex mtx;
    std::condition_variable cv;
    bool uciok_received = false;
    bool readyok_received = false;
    std::string best_move;

    // Reader thread to process Stockfish output
    std::thread reader([&]() {
        std::string line;
        while (std::getline(stockfish_stdout, line)) {
            std::cout << "Stockfish: " << line << std::endl;

            if (line.find("uciok") != std::string::npos) {
                std::lock_guard<std::mutex> lock(mtx);
                uciok_received = true;
                cv.notify_one();
            }
            if (line.find("readyok") != std::string::npos) {
                std::lock_guard<std::mutex> lock(mtx);
                readyok_received = true;
                cv.notify_one();
            }
            if (line.find("bestmove") != std::string::npos) {
                // Extract the best move
                size_t pos = line.find("bestmove");
                if (pos != std::string::npos) {
                    best_move = line.substr(pos + 9, 4); // Assumes standard move format
                    cv.notify_one();
                }
            }
        }
        });

    // Initialize UCI
    stockfish_stdin << "uci" << std::endl;

    // Wait for uciok
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return uciok_received; });
    }

    // Check readiness
    stockfish_stdin << "isready" << std::endl;

    // Wait for readyok
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return readyok_received; });
    }

    // Set position to startpos with some moves
    stockfish_stdin << "position startpos moves e2e4 e7e5 g1f3" << std::endl;

    // Request best move
    stockfish_stdin << "go depth 15" << std::endl;

    // Wait for best move
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !best_move.empty(); });
    }

    QMessageBox msg;
    msg.setText(QString(best_move.c_str()));
    msg.exec();

    // Terminate Stockfish
    stockfish_stdin << "quit" << std::endl;
    stockfish_process.wait();
    reader.join();
}

StockFish::~StockFish(void) {
	
}

std::string StockFish::getNextMove(std::string posFen) {
	chess::Board board(posFen);
	chess::Movelist moveList;
	chess::movegen::legalmoves(moveList, board);
    return "ABC";
}