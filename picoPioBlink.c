#include <stdio.h>

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "picoPioBlink.pio.h"

#define LED_PIN 25

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq);

int main() {
    // UARTのセットアップです
    setup_default_uart();

    // 使うPIOを選択します
    PIO pio = pio0;

    // 命令用のメモリーにアセンブラのプログラムを登録します
    // 第2引数には picoPioBlink.pioファイルで書いたアセンブラコードの ".program
    // の後の名前に & をつけます"
    uint offset = pio_add_program(pio, &picoPioBlink_program);

    // UARTの出力です
    printf("Loaded program at %d\n", offset);

    // 下の関数を呼びます
    blink_pin_forever(pio, 0, offset, LED_PIN, 1);
}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    // 初期化関数を呼びます。picoPioBlink.pio に記述している関数です
    picoPioBlink_program_init(pio, sm, offset, pin);

    // SMを有効にします
    pio_sm_set_enabled(pio, sm, true);

    // UARTの出力です
    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // TX FIFO に遅延時間をセットします
    pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}
