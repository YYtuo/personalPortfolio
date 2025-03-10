核心板V1.2版本

BootLoader
FATFS文件系统，读取SD卡进行程序更新（若更新），并运行用户程序。
直接按 fun_key 不进行loading，直接跳转到板载的默认程序段。
在 3 秒内按下按键 ，打断loading，进入USB读卡器模式，下载bin程序：


应用程序
led_test:最简单的测试代码，按健控制rgb，测试用
core_demo：所有代码的合集

