// #include "main.h"

// #define LHS_IR 22
// #define RHS_IR 21

// #define LHS_W_IN1 23
// #define LHS_W_IN2 19

// #define RHS_W_IN1 25
// #define RHS_W_IN2 33

// void MovementTask(void *pvParameters){
//   pinMode(LHS_IR,INPUT);
//   pinMode(RHS_IR,INPUT);

//   pinMode(LHS_W_IN1, OUTPUT);
//   pinMode(LHS_W_IN2, OUTPUT);

//   pinMode(RHS_W_IN1, OUTPUT);
//   pinMode(RHS_W_IN2, OUTPUT);  
//     while(1){
//         int LHS_IR_VAL = digitalRead(LHS_IR);
//         int RHS_IR_VAL = digitalRead(RHS_IR);

//         if (RHS_IR_VAL == LOW && LHS_IR_VAL == LOW)
//         {
//             digitalWrite(RHS_W_IN1, HIGH);
//             digitalWrite(RHS_W_IN2, LOW);   
//             digitalWrite(LHS_W_IN1, HIGH );
//             digitalWrite(LHS_W_IN2, LOW ); 
//         }
//         //If right sensor detects black line, then turn right
//         else if (RHS_IR_VAL == HIGH && LHS_IR_VAL == LOW )
//         {
//             digitalWrite(RHS_W_IN1, LOW);
//             digitalWrite(RHS_W_IN2, HIGH);   
//             digitalWrite(LHS_W_IN1, LOW );
//             digitalWrite(LHS_W_IN2, LOW ); 
//         }
//         //If left sensor detects black line, then turn left  
//         else if (RHS_IR_VAL == LOW && LHS_IR_VAL == HIGH )
//         {
//             digitalWrite(RHS_W_IN1, LOW);
//             digitalWrite(RHS_W_IN2, LOW);   
//             digitalWrite(LHS_W_IN1, LOW );
//             digitalWrite(LHS_W_IN2, HIGH ); 
//         } 
//         //If both the sensors detect black line, then stop 
//         else 
//         {
//             digitalWrite(RHS_W_IN1, LOW);
//             digitalWrite(RHS_W_IN2, LOW);   
//             digitalWrite(LHS_W_IN1, LOW );
//             digitalWrite(LHS_W_IN2, LOW ); 
//         }
//     }
// }
