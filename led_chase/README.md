# LED đuổi PB0–PB5

Project cho A34M418: LED 1–6 nối lần lượt PB0–PB5. Mỗi lần chỉ một LED sáng, chạy từ LED 1 đến LED 6 rồi lặp lại. Thời gian mỗi bước là 200 ms; sửa `STEP_DELAY_MS` trong `main.c` để đổi tốc độ.

Biên dịch:

```sh
make -C led_chase/a34m41x/gcc
```

Nạp `led_chase/a34m41x/gcc/out/led_chase.hex` bằng J-Flash Lite với device `A34M418`, interface `SWD`. Cấp nguồn cho board qua micro USB của A-Link và nối VTref của J-Link với nguồn tham chiếu 3,3 V của board.
