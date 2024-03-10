#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

const int ROWS = 30; // Số hàng của mê cung
const int COLUMNS = 30; // Số cột của mê cung
const int WALL_RATIO = 5; // Số ô tối thiểu của tường

vector<vector<int>> maze(ROWS, vector<int>(COLUMNS, 0)); // Khởi tạo mê cung với giá trị ban đầu là 0

// Tạo khung mê cung
void CreateMaze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            maze[i][j] = 1; // Gán giá trị 1 cho tất cả các ô trong mê cung, biểu thị cho không gian trống
        }
    }
}
// Tạo tường ngẫu nhiên
void GenerateRandomWalls() {
    // Khởi tạo thiết bị tạo số ngẫu nhiên
    random_device rd;

    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    mt19937 gen(rd());

    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);

    // Tính số lượng bức tường dựa trên tỷ lệ
    int numWalls = (WALL_RATIO * ROWS * COLUMNS) / 75;

    // Đảm bảo ít nhất một bức tường được tạo ra
    numWalls = max(numWalls, 1);

    // Lặp lại để tạo số lượng bức tường mong muốn
    int wallCount = 0; // Số lượng bức tường đã tạo
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

            // Chiều dài ban đầu của bức tường
            int wall_length = 1;

            // Kiểm tra giới hạn trước khi vào vòng lặp để mở rộng bức tường
            if (direction == 0) { // Ngang
                if (column + 5 >= COLUMNS)
                    continue;
            } else { // Dọc
                if (row + 5 >= ROWS)
                    continue;
            }

            // Biến cờ xác định vị trí đặt hợp lệ
            bool valid_placement = true;

            // Lặp lại để mở rộng bức tường cho đến khi đạt độ dài tối đa hoặc gặp chướng ngại vật
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

            // Tạo bức tường hợp lệ
            if (valid_placement) {
                // Lặp lại để đặt các ô theo chiều dài bức tường
                for (int j = 0; j < wall_length; j++) {
                    // Xác định vị trí đặt theo hướng
                    if (direction == 0) {
                        maze[row][column + j] = 0;
                    } else {
                        maze[row + j][column] = 0;
                    }
                }
                wallCount++; // Tăng số lượng bức tường đã tạo
            }
        }
    }
}

// Tìm vị trí các điểm thưởng
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
}


// In ra mê cung
void PrintMaze() {

for (int i = 0; i < ROWS; i++) {
        maze[i][0] = 0; // Cột đầu tiên
        maze[i][COLUMNS - 1] = 0; // Cột cuối cùng
    }
    for (int j = 0; j < COLUMNS; j++) {
        maze[0][j] = 0; // Hàng đầu tiên
        maze[ROWS - 1][j] = 0; // Hàng cuối cùng
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (maze[i][j] == 0) {
                cout << "#" << " "; // Ô có tường
            } else if (maze[i][j] == 1) {
                cout << "." << " "; // Ô trống
            } else if (maze[i][j] == 2) {
                cout << "x" << " "; // Điểm thưởng
            } else if (maze[i][j] == 3) {
                cout << "?" << " "; // Nơi ẩn
            } else if (maze[i][j] == 4) {
                cout << "0" << " "; // Cổng bí mật
            } else {
                cout << "*" << " "; // Vị trí người chơi
            }
        }
        cout << endl;
    }
}

// Di chuyển người chơi
void MovePlayer(int& playerRow, int& playerCol, int& score) {
    char direction;
    cout << "Enter direction (w: up, s: down, a: left, d: right): ";
    cin >> direction;

    switch (direction) {
        case 'w':
            if (playerRow > 0 && maze[playerRow - 1][playerCol] == 1) {
                // Cập nhật vị trí người chơi
                maze[playerRow][playerCol] = 1; // Xóa vị trí cũ của người chơi
                playerRow--;
                if (maze[playerRow][playerCol] == 2) {
                    // Nếu người chơi đến ô số 2, tạo số ngẫu nhiên từ 1 đến 3 và cộng vào tổng điểm
                    int randomScore = rand() % 3 + 1;
                    cout << "You've reached a reward! You got " << randomScore << " points!" << endl;
                    score += randomScore;
                }
                maze[playerRow][playerCol] = 5; // Đánh dấu vị trí người chơi mới trên mê cung
            }
            break;
        case 's':
            if (playerRow < ROWS - 1 && maze[playerRow + 1][playerCol] == 1) {
                // Cập nhật vị trí người chơi
                maze[playerRow][playerCol] = 1; // Xóa vị trí cũ của người chơi
                playerRow++;
                if (maze[playerRow][playerCol] == 2) {
                    // Nếu người chơi đến ô số 2, tạo số ngẫu nhiên từ 1 đến 3 và cộng vào tổng điểm
                    int randomScore = rand() % 3 + 1;
                    cout << "You've reached a reward! You got " << randomScore << " points!" << endl;
                    score += randomScore;
                }
                maze[playerRow][playerCol] = 5; // Đánh dấu vị trí người chơi mới trên mê cung
            }
            break;
        case 'a':
            if (playerCol > 0 && maze[playerRow][playerCol - 1] == 1) {
                // Cập nhật vị trí người chơi
                maze[playerRow][playerCol] = 1; // Xóa vị trí cũ của người chơi
                playerCol--;
                if (maze[playerRow][playerCol] == 2) {
                    // Nếu người chơi đến ô số 2, tạo số ngẫu nhiên từ 1 đến 3 và cộng vào tổng điểm
                    int randomScore = rand() % 3 + 1;
                    cout << "You've reached a reward! You got " << randomScore << " points!" << endl;
                    score += randomScore;
                }
                maze[playerRow][playerCol] = 5; // Đánh dấu vị trí người chơi mới trên mê cung
            }
            break;
        case 'd':
            if (playerCol < COLUMNS - 1 && maze[playerRow][playerCol + 1] == 1) {
                // Cập nhật vị trí người chơi
                maze[playerRow][playerCol] = 1; // Xóa vị trí cũ của người chơi
                playerCol++;
                if (maze[playerRow][playerCol] == 2) {
                    // Nếu người chơi đến ô số 2, tạo số ngẫu nhiên từ 1 đến 3 và cộng vào tổng điểm
                    int randomScore = rand() % 3 + 1;
                    cout << "You've reached a reward! You got " << randomScore << " points!" << endl;
                    score += randomScore;
                }
                maze[playerRow][playerCol] = 5; // Đánh dấu vị trí người chơi mới trên mê cung
            }
            break;
        default:
            cout << "Invalid direction!" << endl;
    }
}

// Hàm main
int main() {
    // Khởi tạo seed cho bộ tạo số ngẫu nhiên
    srand(time(nullptr));

    CreateMaze(); // Tạo mê cung
    GenerateRandomWalls(); // Tạo tường ngẫu nhiên
    FindRewards(); // Tìm vị trí các điểm thưởng

    int playerRow = 1; // Vị trí hàng ban đầu của người chơi
    int playerCol = 1; // Vị trí cột ban đầu của người chơi
    int score = 0; // Tổng điểm của người chơi
    maze[playerRow][playerCol] = 5; // Vị trí ban đầu của người chơi

    // In ra mê cung ban đầu với vị trí người chơi
    PrintMaze();
    cout << endl;

    // Tiến hành di chuyển người chơi
    while (maze[playerRow][playerCol] != 2) {
        MovePlayer(playerRow, playerCol, score); // Di chuyển người chơi
        PrintMaze(); // In ra mê cung sau mỗi bước di chuyển
        cout << endl;
    }

    cout << "Congratulations! You've reached the destination!" << endl;
    cout << "Your total score is: " << score << endl;

    return 0;
}
