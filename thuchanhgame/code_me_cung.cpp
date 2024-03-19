#include <algorithm>
#include <random>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "code_me_cung.h"

clock_t startTime; // Thời gian bắt đầu của trò chơi

// Hàm này sẽ tính thời gian còn lại
int TimeLeft() {
    clock_t now = clock();
    int elapsedTime = (now - startTime) / CLOCKS_PER_SEC;
    return GAME_DURATION - elapsedTime;
}

std::vector<std::vector<int>> maze(ROWS, std::vector<int>(COLUMNS, 0)); // Mảng lưu trữ mê cung

// Tạo mê cung ban đầu
void CreateMaze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            maze[i][j] = 1; // Gán giá trị 1 cho tất cả các ô trong mê cung, biểu thị cho không gian trống
        }
    }
}

// Tạo tường ngẫu nhiên trong mê cung
void GenerateRandomWalls() {
    // Khởi tạo thiết bị tạo số ngẫu nhiên
    std::random_device rd;

    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    std::mt19937 gen(rd());

    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    std::uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);

    // Tính số lượng ô tường cần tạo dựa trên tỷ lệ
    int numWalls = (WALL_RATIO * ROWS * COLUMNS) / 75;

    // Đảm bảo ít nhất một ô tường được tạo ra
    numWalls = std::max(numWalls, 1);

    // Số lượng ô tường đã được tạo
    int wallCount = 0;

    // Lặp lại để tạo các ô tường ngẫu nhiên
    while (wallCount < numWalls) {
        // Tạo chỉ số ngẫu nhiên trong phạm vi mảng
        int index = dist(gen);

        // Tính vị trí hàng và cột từ chỉ số
        int row = index / COLUMNS;
        int column = index % COLUMNS;

        // Kiểm tra ô hợp lệ (trống và nằm trong giới hạn mê cung)
        if (maze[row][column] == 1 && row > 0 && row < ROWS - 1 && column > 0 && column < COLUMNS - 1) {
            // Chọn hướng ngẫu nhiên (ngang = 0, dọc = 1)
            int direction = gen() % 2;

            // Độ dài ban đầu của ô tường
            int wall_length = 1;

            // Kiểm tra giới hạn trước khi vào vòng lặp để mở rộng ô tường
            if (direction == 0) { // Ngang
                if (column + 5 >= COLUMNS)
                    continue;
            } else { // Dọc
                if (row + 5 >= ROWS)
                    continue;
            }

            // Biến cờ xác định vị trí đặt hợp lệ
            bool valid_placement = true;

            // Lặp lại để mở rộng ô tường cho đến khi đạt độ dài tối đa hoặc gặp chướng ngại vật
            while (wall_length < 5 && valid_placement) {
                // Xác định hướng mở rộng
                if (direction == 0) { // Ngang
                    // Kiểm tra ô tiếp theo
                    if (maze[row][column + wall_length] != 1) {
                        valid_placement = false;
                    } else {
                        wall_length++;
                    }
                } else { // Dọc
                    // Kiểm tra ô tiếp theo
                    if (maze[row + wall_length][column] != 1) {
                        valid_placement = false;
                    } else {
                        wall_length++;
                    }
                }
            }

            // Tạo ô tường hợp lệ
            if (valid_placement) {
                // Lặp lại để đặt các ô theo chiều dài ô tường
                for (int j = 0; j < wall_length; j++) {
                    // Xác định vị trí đặt theo hướng
                    if (direction == 0) {
                        maze[row][column + j] = 0;
                    } else {
                        maze[row + j][column] = 0;
                    }
                }
                wallCount++; // Tăng số lượng ô tường đã tạo
            }
        }
    }
}

// Tìm vị trí các ô có điểm thưởng
void FindRewards() {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 1) {
                int wallCount = 0;
                // Kiểm tra các ô xung quanh
                if (maze[i][j - 1] == 0 ) {
                    wallCount++;
                }
                if (maze[i][j + 1] == 0 ) {
                    wallCount++;
                }
                if (maze[i + 1][j] == 0 ) {
                    wallCount++;
                }
                if ( maze[i - 1][j] == 0 ) {
                    wallCount++;
                }
                // Nếu có 3 ô trống xung quanh, đánh dấu ô hiện tại
                if (wallCount == 3) {
                    maze[i][j] = 2;
                }
            }
        }
    }

    // Kiểm tra ô xung quanh các ô có điểm thưởng để đánh dấu các ô nơi ẩn
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 1) {
                int count = 0;
                // Kiểm tra các ô xung quanh
                if (maze[i][j - 1] == 0 && maze[i-1][j] == 0) {
                    count++;
                }
                 if (maze[i][j + 1] == 0 && maze[i-1][j] == 0) {
                    count++;
                }
                 if (maze[i][j - 1] == 0 && maze[i+1][j] == 0) {
                    count++;
                }
                 if (maze[i][j + 1] == 0 && maze[i+1][j] == 0) {
                    count++;
                }
                // Nếu có 3 ô trống xung quanh, đánh dấu ô hiện tại
                if (count == 1) {
                    maze[i][j] = 3;
                }
            }
        }
    }
}

// Sửa lỗi mê cung
void FixMazeerror() {
    int i = 1;
    int j = 1;
    for (int di = 0; di <= 3; di++) {
        for (int dj = 0; dj <= 3; dj++) {
            // Kiểm tra xem truy cập có vượt ra ngoài giới hạn không
            if (i + di < maze.size() && j + dj < maze[i].size()) {
                maze[i + di][j + dj] = 1;
            }
        }
    }
}

// Xử lý ô trống không kề tường
void BreakWalls() {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 0) {
                int count = 0;
                if (maze[i][j + 1] == 0 || maze[i][j - 1] == 0) {
                    count = 1;
                }
                if (count == 0) {
                    maze[i][j] = 4;
                } else {
                    if (maze[i + 1][j] == 0 || maze[i - 1][j] == 0) {
                        count = 2;
                    }
                    if (count == 1) {
                        maze[i][j] = 4;
                    }
                }
            }
        }
    }
}

// In ra mê cung với vị trí đích
void PrintMazeWithDestination(int endRow, int endCol) {
    static bool printedOnce = false; // Biến đánh dấu đã in mê cung ít nhất một lần
    if (!printedOnce) { // Kiểm tra nếu chưa in mê cung ít nhất một lần
        int rewards = 0, hiddens = 0;
        // Tính toán giá trị của rewards và hiddens
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (maze[i][j] == 2) {
                    rewards++;
                } else if (maze[i][j] == 3) {
                    hiddens++;
                }
            }
        }

        // Đánh dấu các cạnh của mê cung
        for (int i = 0; i < ROWS; i++) {
            maze[i][0] = 0; // Cột đầu tiên
            maze[i][COLUMNS - 1] = 0; // Cột cuối cùng
        }
        for (int j = 0; j < COLUMNS; j++) {
            maze[0][j] = 0; // Hàng đầu tiên
            maze[ROWS - 1][j] = 0; // Hàng cuối cùng
        }

        // In ra mê cung với các ký hiệu tương ứng
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (i == endRow && j == endCol) {
                    std::cout << "D" << " "; // In ra vị trí của điểm đích
                } else if (maze[i][j] == 0) {
                    std::cout << "#" << " "; // Ô có tường
                } else if (maze[i][j] == 1) {
                    std::cout << "." << " "; // Ô trống
                } else if (maze[i][j] == 2) {
                    std::cout << "x" << " "; // Điểm thưởng
                } else if (maze[i][j] == 3) {
                    std::cout << "?" << " "; // Nơi ẩn
                } else if (maze[i][j] == 4) {
                    std::cout << "0" << " "; // Cổng bí mật
                } else {
                    std::cout << "*" << " "; // Vị trí người chơi
                }
            }
            std::cout << std::endl;
        }

        // In ra tổng số lượng rewards và hiddens trong mê cung
        std::cout << "Rewards: " << rewards << std::endl;
        std::cout << "Hiddens: " << hiddens << std::endl;

        printedOnce = true; // Đánh dấu đã in mê cung ít nhất một lần
    } else { // Nếu đã in mê cung ít nhất một lần, chỉ in mê cung mà không tính toán lại rewards và hiddens
        // In ra mê cung với các ký hiệu tương ứng
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (i == endRow && j == endCol) {
                    std::cout << "D" << " "; // In ra vị trí của điểm đích
                } else if (maze[i][j] == 0) {
                    std::cout << "#" << " "; // Ô có tường
                } else if (maze[i][j] == 1) {
                    std::cout << "." << " "; // Ô trống
                } else if (maze[i][j] == 2) {
                    std::cout << "x" << " "; // Điểm thưởng
                } else if (maze[i][j] == 3) {
                    std::cout << "?" << " "; // Nơi ẩn
                } else if (maze[i][j] == 4) {
                    std::cout << "0" << " "; // Cổng bí mật
                } else {
                    std::cout << "*" << " "; // Vị trí người chơi
                }
            }
            std::cout << std::endl;
        }
        // In ra thông tin về thời gian còn lại
        int timeRemaining = TimeLeft();
        std::cout << "Time remaining: " << timeRemaining << " seconds" << std::endl;
    }
}

void MovePlayer(int& playerRow, int& playerCol, int& score, int& breakCount, int& hiddenCount) {
    char direction;
    std::cout << "Nhập hướng di chuyển (w: lên, s: xuống, a: trái, d: phải): ";
    std::cin >> direction;

    switch (direction) {
        case 'w':
            if (playerRow > 0 && maze[playerRow - 1][playerCol] != 0) {
                if (maze[playerRow - 1][playerCol] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow - 1][playerCol] = 1;
                    } else {
                        std::cout << "Bạn không có đủ lượt phá tường để đi qua tường này!" << std::endl;
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerRow--;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    std::cout << "Bạn đã đạt được phần thưởng! Bạn nhận được " << randomScore << " điểm!" << std::endl;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    breakCount ++;
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else {
                std::cout << "bạn không thể phá hay đi qua tường này " << std::endl;
            }

            break;
        case 's':
           if (playerRow < ROWS - 1 && maze[playerRow + 1][playerCol] != 0) {
                if (maze[playerRow + 1][playerCol] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow + 1][playerCol] = 1;
                    } else {
                        std::cout << "Bạn không có đủ lượt phá tường để đi qua tường này!" << std::endl;
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerRow++;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    std::cout << "Bạn đã đạt được phần thưởng! Bạn nhận được " << randomScore << " điểm!" << std::endl;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    breakCount ++;
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else {
                std::cout << "bạn không thể phá hay đi qua tường này " << std::endl;
            }

            break;
        case 'a':
            if (playerCol > 0 && maze[playerRow][playerCol - 1] != 0) {
                if (maze[playerRow][playerCol - 1] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow][playerCol - 1] = 1;
                    } else {
                        std::cout << "Bạn không có đủ lượt phá tường để đi qua tường này!" << std::endl;
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerCol--;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    std::cout << "Bạn đã đạt được phần thưởng! Bạn nhận được " << randomScore << " điểm!" << std::endl;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    breakCount ++;
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else {
                std::cout << "bạn không thể phá hay đi qua tường này " << std::endl;
            }

            break;
        case 'd':
            if (playerCol < COLUMNS - 1 && maze[playerRow][playerCol + 1] != 0) {
                if (maze[playerRow][playerCol + 1] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow][playerCol + 1] = 1;
                    } else {
                        std::cout << "Bạn không có đủ lượt phá tường để đi qua tường này!" << std::endl;
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerCol++;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    std::cout << "Bạn đã đạt được phần thưởng! Bạn nhận được " << randomScore << " điểm!" << std::endl;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    breakCount ++;
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else {
                std::cout << "bạn không thể phá hay đi qua tường này " << std::endl;
            }

            break;
        default:
            std::cout << "Hướng di chuyển không hợp lệ!" << std::endl;
            break;
    }
}

// Tìm một ô trống ở cuối mê cung đẻ làm đích
std::pair<int, int> FindEmptySpace() {
  int endRow = ROWS - 2; // Hàng cuối cùng
  int endCol = COLUMNS - 2; // Cột cuối cùng

  // Lặp qua các ô trong mê cung từ cuối mê cung lên trên và từ phải sang trái
  for (int i = endRow; i >= 0; i--) {
    for (int j = endCol; j >= 0; j--) {
      // Nếu ô hiện tại là trống và không phải là tường, trả về vị trí ô đó
      if (maze[i][j] == 1) {
        return std::make_pair(i, j);
      }
    }
  }

  // Nếu không tìm thấy ô trống, trả về vị trí (0, 0) (không hợp lệ)
  return std::make_pair(0, 0);
}

int run() {
    srand(time(nullptr));

    CreateMaze();
    GenerateRandomWalls();
    FixMazeerror();
    FindRewards();
    BreakWalls();

    int playerRow = 1;
    int playerCol = 1;
    int score = 0;
    int breakCount = 3;
    int hiddenCount = 0;
    maze[playerRow][playerCol] = 5;

    std::pair<int, int> endPosition = FindEmptySpace();
    int endRow = endPosition.first;
    int endCol = endPosition.second;

    maze[endRow][endCol] = 6;

    startTime = clock();

    while (playerRow != endRow || playerCol != endCol) {
        int timeRemaining = TimeLeft();
        if (timeRemaining <= 0) {
            std::cout << "Hết thời gian! Game over." << std::endl;
            break;
        }
        system("cls");
        PrintMazeWithDestination(endRow, endCol);
        std::cout << "Total Score: " << score << ", Break Count: " << breakCount << ", Hidden Count: " << hiddenCount << std::endl;

        MovePlayer(playerRow, playerCol, score, breakCount, hiddenCount);
    }

    PrintMazeWithDestination(endRow, endCol);
    std::cout << "Total Score: " << score << ", Break Count: " << breakCount << ", Hidden Count: " << hiddenCount << std::endl;

    std::cout << "Congratulations! You've reached the destination!" << std::endl;
    std::cout << "Your total score is: " << score << std::endl;

    return 0;
}

