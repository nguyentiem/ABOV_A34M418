# STK-A34M418-VLN-A: trạng thái build, debug và test

Cập nhật: 2026-10-26

## Đã kiểm tra

- Có tài liệu VLN trong `docs/`: manual starter kit, datasheet và user manual A34M41x.
- Có SDK `AUDK32_A34xxxx-1.0.13` và gói ví dụ `RSW_StarterKit-A34M418YLN`. Gói ví dụ mang tên **YLN**, cần kiểm tra khác biệt chân và cấu hình trước khi dùng trên **VLN**.
- Máy có ARM GCC 13.2.1, `make`, `gdb-multiarch`, J-Link Commander, J-Link GDB Server và Ozone 3.50a. `lsusb` thấy J-Link PLUS.
- Đã build thành công ví dụ LED blinky bằng lệnh:

  ```sh
  make -C SDK/RSW_StarterKit-A34M418YLN/example/starterkit/gpio_ledblinky/a34m41x/gcc -j2
  ```

- Kết quả: `.../gcc/out/app.elf`, `app.hex`, `app.bin`. Có cảnh báo syscall `_close`, `_fstat`, `_isatty`, `_lseek` từ newlib/nosys nhưng build thành công.
- Linker script ví dụ khai báo 512 KiB code flash và 64 KiB RAM, khớp manual VLN.
- SEGGER liệt kê A34M418 là thiết bị được J-Link hỗ trợ; Ozone có thể debug ELF từ GCC qua J-Link PLUS và SWD.
- SDK AUDK32 có OpenOCD hỗ trợ A34M41x. Target `make flash` trong gói RSW trỏ tới `tools/openocd` không có trong gói này, nên chưa dùng được trực tiếp.

## Chưa xác nhận

- Lúc kiểm tra `lsusb` chưa thấy board VLN. Chưa kết nối SWD tới chip, chưa nạp firmware, chưa thử breakpoint hoặc chạy test trên board.
- Chưa đối chiếu chân LED/UART/clock giữa ví dụ YLN và board VLN.

## Tiếp tục lần sau

1. Cấp nguồn board VLN. Nối J-Link PLUS tới SWD: VTref, GND, SWDIO, SWCLK; xác nhận chiều chân theo `docs/STK_Manual_A34M418VLN_Eng.pdf`.
2. Dùng J-Link Commander xác nhận kết nối, chọn device `A34M418`, interface `SWD`, đọc ID/ thanh ghi; chưa cần nạp flash.
3. Mở Ozone, tạo project device `A34M418`, probe J-Link, SWD, chọn ELF ở trên. Thử halt, step và breakpoint.
4. Đối chiếu cấu hình ví dụ YLN với sơ đồ VLN, rồi mới nạp và kiểm tra LED hoặc bài test khác.

Nguồn tham khảo: https://www.segger.com/supported-devices/abov/ ; https://kb.segger.com/Starter_Kit_A34M418VLN_v1.2
