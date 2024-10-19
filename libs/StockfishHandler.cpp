#include "StockfishHandler.h"
#include "chess.hpp"
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
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
}

std::string StockFish::getNextMove(std::string posFen) {
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
#ifdef _WIN32
        , bp::windows::hide  // This hides the console window on Windows
#endif
    );

    std::mutex mtx;
    std::condition_variable cv;
    std::string best_move = "";

    // Reader thread to process Stockfish output
    std::thread reader([&]() {
        std::string line;
        while (std::getline(stockfish_stdout, line)) {
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

    int depth, skill;
    switch (this->mode) {
    case 1:
        depth = 1;
        skill = 1;
        break;
    case 2:
        depth = 5;
        skill = 10;
        break;
    case 3:
        depth = 7;
        skill = 20;
        break;
    default:
        depth = 5;
        skill = 5;
        break;
    }


    // Set position to startpos with some moves
    stockfish_stdin << "setoption name Skill Level value " + std::to_string(skill) << std::endl;
    stockfish_stdin << "position fen " + posFen << std::endl;

    // Request best move
    stockfish_stdin << "go depth " + std::to_string(skill) << std::endl;

    // Wait for best move
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !best_move.empty(); });
    }

    //QMessageBox msg;
    //msg.setText(QString(best_move.c_str()));
    //msg.exec();

    // Terminate Stockfish
    stockfish_stdin << "quit" << std::endl;
    stockfish_process.wait();
    reader.join();
    return best_move;
}

StockFish::~StockFish(void) {

}

//std::string StockFish::getNextMove(std::string posFen) {
//	chess::Board board(posFen);
//	chess::Movelist moveList;
//	chess::movegen::legalmoves(moveList, board);
//    return "ABC";
//}