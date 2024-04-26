tất cả các hìnha nrh sử dụng trong file này đều là do copilot vẽ và sau đó sửa lại 1 số ảnh bằng cách xóa nền
đoạn code timer của file time.h và time.cpp là tham khảo code từ link lazyfoo dạy học từ cơ bản này  https://www.lazyfoo.net/SDL_tutorials/index.php và link của timer là https://www.lazyfoo.net/SDL_tutorials/lesson13/index.php
đoạn code này có hỏi chatgpt về cách tạo ra mấu ngẫu nhiên nằm trong hàm GenerateRandomWalls() của classs Maze
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    std::mt19937 gen(tp.time_since_epoch().count());
    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    std::uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);


1, khi bắt đầu game màn hình sẽ hiện ra
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/efe51c61-de68-4f92-9237-d6c1c1690dcc)

để chơi bấm vào khu vực phím "START" bằng chuột trái
2, khi vào chơi thì sẽ có giao diện như thế này 
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/f4b2c751-b2df-4741-aac6-32e13f1e3170)
phần bên trái này sẽ là phần mê cung của người chơi và di chuyển bằng cách bấm phím 'w' để di chuyển lên, 's' để di chuyển xuống, 'd' để di chuyển sang phải, 'a' để di chuyển sang trái
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/c2baa681-6832-48a4-a3b8-52c0f9029be5)
tường màu đỏ là tường không thể phá được
tường màu tím là tường có thể phá được
ô màu xanh là đường có thể đi qua;
những chiếc rương thưởng là 1 món quà bí mật, bao gồm bom, điểm phá tường, điểm
rường ở cuối cùng là rương thưởng tượng trưng cho chiến 
phần bên phải này là chức năng 
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/4a62e7dd-baca-440c-af37-669f9372f272)
bao gồm thời gian đã chơi, máu người chơi mặc định là 500, điểm số, số rương ăn được, và số lần phá tường, số bước di chuyển run.
3, khi muốn tạm dừng thì bấm phím 'q' sẽ cho tạm dừng và hiện lên thông báo
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/601347fb-8846-4d36-9555-cd8b89826dd3)
nếu nhấn tiếp phím 'q' thì trò chơi sẽ được tiếp tục 
còn nếu nhấn phím 'r' thì sẽ xóa hết mê cung cũ và hiện lên ready và đếm ngược 3 giây trước khi bắt đầu trò chơi mới
khi không muốn chơi nữa thì sẽ bấm nút tắt ở trên cùng màn hình bên phải thì chương trình sẽ hiện lên thông báo loss
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/768f10bd-ba6b-41db-ae10-5828b8fdfa9f)
và sau đó là thông báo hỏi xem bạn có muốn chơi nữa không 
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/0eb86dc5-a6c2-4c60-8ee6-fb52180bda2c)
nếu bạn bấm vào phím 'y' thì nó sẽ đưa ra trờ chơi mới cho bạn còn nếu bấm phím 'n' thì chỉ cần chờ 2 giây chương trình sẽ tự kết thúc 








