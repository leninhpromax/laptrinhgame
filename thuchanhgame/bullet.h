#include <cmath>
#include <vector>
#include "player.h"

class Bullet {
private:
    int currentRow;
    int currentCol;
    bool isActive;
    float shootingAngle;
    int startRow;
    int startCol;
    bool playerMovedUp;
    bool playerMovedDown;
    bool playerMovedLeft;
    bool playerMovedRight;
    int bulletCount; // Số lượng viên đạn còn

public:
    Bullet() : currentRow(0), currentCol(0), isActive(false), shootingAngle(0.0f), bulletCount(5) {}

    // Bắn viên đạn từ vị trí (startRow, startCol) dựa trên hướng di chuyển của người chơi
    void Shoot(int startRow, int startCol, bool playerMovedUp, bool playerMovedDown, bool playerMovedLeft, bool playerMovedRight) {
        if (bulletCount > 0) { // Kiểm tra xem còn viên đạn không
            currentRow = startRow;
            currentCol = startCol;
            this->startRow = startRow;
            this->startCol = startCol;
            // Xác định góc bắn dựa trên hướng di chuyển của người chơi
            if (playerMovedUp) {
                shootingAngle = -M_PI / 2; // Bắn lên trên
            } else if (playerMovedDown) {
                shootingAngle = M_PI / 2; // Bắn xuống dưới
            } else if (playerMovedLeft) {
                shootingAngle = M_PI; // Bắn sang trái
            } else if (playerMovedRight) {
                shootingAngle = 0; // Bắn sang phải
            }

            isActive = true;
            bulletCount--; // Giảm số lượng viên đạn sau khi bắn
        }
    }

    // Di chuyển viên đạn theo góc đã bắn
    void Move(std::vector<std::vector<int>>& maze) {
        if (isActive) {
            float bulletSpeed = 1.0f; // Tốc độ của viên đạn

            // Tính toán vị trí mới dựa trên góc bắn
            float deltaX = bulletSpeed * cos(shootingAngle); // Thay đổi theo hướng x (cột)
            float deltaY = bulletSpeed * sin(shootingAngle); // Thay đổi theo hướng y (hàng)

            // Tính toán vị trí mới của đạn
            float newX = currentCol + deltaX;
            float newY = currentRow + deltaY;

            int newRow = static_cast<int>(newY);
            int newCol = static_cast<int>(newX);

            if (maze[newRow][newCol] != 0) {
                // Nếu viên đạn không va chạm với tường
                if (maze[newRow][newCol] == 4) {
                    // Nếu viên đạn va chạm với tường có giá trị 4
                    maze[newRow][newCol] = 1; // Biến tường thành không gian trống
                } else {
                    currentRow = newRow;
                    currentCol = newCol;
                }
            } else {
                isActive = false; // Khi viên đạn va chạm, vô hiệu hóa nó
            }
        }
    }

    // Kiểm tra xem viên đạn có đang hoạt động hay không
    bool IsActive() const {
        return isActive;
    }

    // Lấy hàng của viên đạn
    int GetRow() const {
        return currentRow;
    }

    // Lấy cột của viên đạn
    int GetCol() const {
        return currentCol;
    }

    int GetBulletCount() {
        return bulletCount;
    }
    // Phương thức để tăng số lượng viên đạn lên 1 đơn vị
    void plusBulletCount() {
        bulletCount++;
    }
};
