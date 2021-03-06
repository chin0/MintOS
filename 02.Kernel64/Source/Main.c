#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"

void kPrintString(int iX, int iY, const char* pcString);

//C언어 커널의 시작 부분
void Main(void) 
{
    char vcTemp[2] = {0,};
    BOOL bFlags;
    BYTE bTemp;
    int i = 0;

    kPrintString(0,10, "Switch To IA-32e Mode Success~!!");
    kPrintString(0,11,"IA-32e C Language Kernel Start........[Pass]");

    kPrintString(0,12,"GDT Initialize and switch for IA-32e mode...[    ]");
    kInitializeGDTTableAndTSS();
    kLoadGDTR(GDTR_STARTADDRESS);
    kPrintString(46,12,"Pass");


    kPrintString(0,13,"Tss Segment Load...............[    ]");
    kLoadTR(GDT_TSSSEGMENT);
    kPrintString(34,13,"Pass");

    kPrintString(0,14,"IDT Initialize.................[    ]");
    kInitializeIDTTables();
    kLoadIDTR(IDTR_STARTADDRESS);
    kPrintString(34,14,"Pass");

    kPrintString(0, 15, "Keyboard Activate...............[    ]");
    if(kActivateKeyboard() == TRUE) {
        kPrintString(33,15, "Pass");
        kChangeKeyboardLED(FALSE,FALSE,FALSE);
    } else {
        kPrintString(33,15, "Fail");
        while(1);
    }

    while(1) {
        //출력 버퍼가 차있으면 스캔 코드를 읽을수 있음
        if(kIsOutputBufferFull() == TRUE) {
            bTemp = kGetKeyboardScanCode();

            //스캔 코드를 아스키 코드로 변환하는 함수를 호출할려 ASCII 코드와
            //눌림 또는 떨어짐 정보를 반환
            if(kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE) {
                if(bFlags & KEY_FLAGS_DOWN) {
                    kPrintString(i++, 16, vcTemp);
                }

                //0입력하면 0으로 나누는 Divide Error를 발생시켜서 에러 핸들러 테스트.
                if(vcTemp[0] == '0') {
                    bTemp = bTemp / 0;
                }
            }
        }
    }
}

void kPrintString(int iX, int iY, const char* pcString)
{
    CHARACTER* pstScreen = (CHARACTER*) 0xB8000;
    int i;

    pstScreen += (iY * 80) + iX;

    for(i = 0; pcString[i] != 0; i++)
    {
        pstScreen[i].bCharactor = pcString[i];
    }
}
