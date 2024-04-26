#include <cmath>
#include <vector>
#include "player.h"

class Bullet {
private:
    int row;
    int col;
    bool active;
    float angle;
    int startRow;
    int startCol;
    bool playerMovedUp;
    bool playerMovedDown;
    bool playerMovedLeft;
    bool playerMovedRight;

public:
    Bullet() : row(0), col(0), active(false), angle(0.0f) {}

    // Bắn viên đạn từ vị trí (startRow, startCol) dựa trên hướng di chuyển của người chơi
    void Shoot(int startRow, int startCol, bool playerMovedUp, bool playerMovedDown, bool playerMovedLeft, bool playerMovedRight) {
        row = startRow;
        col = startCol;
        this->startRow = startRow;
        this->startCol = startCol;
        // Xác định góc bắn dựa trên hướng di chuyển của người chơi
        if (playerMovedUp) {
            angle = -M_PI / 2; // Bắn lên trên
        } else if (playerMovedDown) {
            angle = M_PI / 2; // Bắn xuống dưới
        } else if (playerMovedLeft) {
            angle = M_PI; // Bắn sang trái
        } else if (playerMovedRight) {
            angle = 0; // Bắn sang phải
        }

        active = true;
    }

    // Di chuyển viên đạn theo góc đã bắn
    void Move(std::vector<std::vector<int>>& maze) {
        if (active) {
            float bulletSpeed = 1.0f; // Tốc độ của viên đạn

            // Tính toán vị trí mới dựa trên góc bắn
            float deltaX = bulletSpeed * cos(angle); // Thay đổi theo hướng x (cột)
            float deltaY = bulletSpeed * sin(angle); // Thay đổi theo hướng y (hàng)

            // Tính toán vị trí mới của đạn
            float newX = col + deltaX;
            float newY = row + deltaY;

            int newRow = static_cast<int>(newY);
            int newCol = static_cast<int>(newX);

            if (maze[newRow][newCol] != 0 && maze[newRow][newCol] != 4) {
                // Nếu viên đạn không va chạm với tường hoặc cổng bí mật
                row = newRow;
                col = newCol;
            } else {
                active = false; // Khi viên đạn va chạm, vô hiệu hóa nó
            }
        }
    }

    // Kiểm tra xem viên đạn có đang hoạt động hay không
    bool isActive() const {
        return active;
    }

    // Lấy hàng của viên đạn
    int getRow() const {
        return row;
    }

    // Lấy cột của viên đạn
    int getCol() const {
        return col;
    }
};
