Cách tải game tải game tại link https://drive.google.com/drive/u/0/folders/17AnR_VrDLFIWYxgkeywS2zZDdmliBWtg
 hoặc có thể tải trực  tại đường link này https://github.com/leninhpromax/laptrinhgame
 ![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/20ef1fac-99a6-485c-94b2-4c5d2f212ab9)

 
Tất cả các hình ảnh sử dụng trong file này đều là do copilot vẽ và sau đó sửa lại 1 số ảnh bằng cách xóa nền
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/16202444-71a3-4455-b222-3868b9d9a790)

Đoạn code timer của file time.h và time.cpp là tham khảo code từ link lazyfoo dạy học từ cơ bản này  https://www.lazyfoo.net/SDL_tutorials/index.php và link của timer là https://www.lazyfoo.net/SDL_tutorials/lesson13/index.php
Đoạn code này có hỏi chatgpt về cách tạo ra mấu ngẫu nhiên nằm trong hàm GenerateRandomWalls() của classs Maze
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    std::mt19937 gen(tp.time_since_epoch().count());
    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    std::uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);
Chương trình vẫn chưua được hoàn thiện bị thiếu đi tính năng quái, câu đố

Phần code tìm chìa khóa midemtyspace có tham khảo sự hỗ trợ của chatgpt

Đường link đến video là https://www.youtube.com/watch?v=mce5OUTSCPw

1. GIÓI THIỆU TRỰC TIẾP VÀ CÁCH NHẬN BIẾT CÁC MỤC TIÊU
a, khi bắt đầu game màn hình sẽ hiện ra
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/efe51c61-de68-4f92-9237-d6c1c1690dcc)

để chơi bấm vào khu vực phím "START" bằng chuột trái
b, khi vào chơi thì sẽ có giao diện như thế này 
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
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/01441dcf-07a8-4f91-a079-d98b4fb34d63)
thực hiện nhấn phím space để bắn đạn 
c, khi muốn tạm dừng thì bấm phím 'q' sẽ cho tạm dừng và hiện lên thông báo
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/601347fb-8846-4d36-9555-cd8b89826dd3)
nếu nhấn tiếp phím 'q' thì trò chơi sẽ được tiếp tục 
còn nếu nhấn phím 'r' thì sẽ xóa hết mê cung cũ và hiện lên ready và đếm ngược 3 giây trước khi bắt đầu trò chơi mới
khi không muốn chơi nữa thì sẽ bấm nút tắt ở trên cùng màn hình bên phải thì chương trình sẽ hiện lên thông báo loss
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/768f10bd-ba6b-41db-ae10-5828b8fdfa9f)
và sau đó là thông báo hỏi xem bạn có muốn chơi nữa không 
![image](https://github.com/leninhpromax/laptrinhgame/assets/90245106/0eb86dc5-a6c2-4c60-8ee6-fb52180bda2c)
nếu bạn bấm vào phím 'y' thì nó sẽ đưa ra trờ chơi mới cho bạn còn nếu bấm phím 'n' thì chỉ cần chờ 2 giây chương trình sẽ tự kết thúc 

2, CẬP NHẬT LOGIC GAME VÀ CÁCH CHƠI GAME 
Mục tiêu cần tìm là chìa khóa và rương đích
+ chìa khóa thì nằm ở nơi bất kì nên sẽ khó hơn do nằm ở vị trí bất kì
+ rương luốn nằm ở dưới cùng mê cung bên phải
+ có được chìa khóa mới mở được rương và chiến thắng
Các biến liên quan
+ tường màu đỏ là tường không thể phá
+ tường màu tím có thể phá được nếu có breakcount hoặc bullet
+ những ô màu xanh lá là những ô mình có thể di chuyển bình thường
+ đạn có màu đen và bắn đạn bằng cách nhấn phím space còn đạn sẽ di chuyển theo đường thẳng, hướng là hướng quay trước khi bắn và sẽ phá hủy tất cả tường tím trên đường nó đi và chỉ mất khi gặp tường đỏ (những vật phẩm khác không bị ảnh hương)
+ những hình cô tiên sẽ giúp được cộng từ 1 đến 3 điểm
+ khi ăn rương trên đường đi sẽ được thưởng 1 breakcount hoặc 1 bullet hoặc cộng thêm 50 máu hoặc cộng thêm 1 đến 3 điểm nhưng nếu đen thì bạn sẽ bị dính bom ẩn trừ từ 10 đến 100 máu
+ khi máu về 0 thì trò chơi sẽ kết thúc
+ khi nào mà người chơi không muốn chơi nữa có thể bấm phím tắt ở phía trên màn hình và chờ đợi để hiện thông báo 'do you ưant to continue' và nhấn phím 'y' để chơi mới hoặc là nhấn phím 'n' để thoát
+ mỗi lần di chuyển sẽ cộng thêm 1 vào biến run đến số bước di chuyển








