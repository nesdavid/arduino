#define IN_DOWN 2
#define IN_LEFT 3 
#define IN_UP 4  
#define IN_MAX IN_UP + 1 

#define OUT_DOWN 8
#define OUT_LEFT 9 
#define OUT_UP 10  
#define OUT_MAX OUT_UP + 1
#define OUT_FIRST OUT_DOWN

#define IN_TO_OUT (OUT_DOWN - IN_DOWN)

#define MAX_STEPS 20
int steps[MAX_STEPS];
int cur_step;

void setup() 
{
  Serial.begin(9600);
  for (int i=IN_DOWN; i<=IN_UP; ++i)
  {
    pinMode(i, INPUT);
  }

  for (int i=OUT_DOWN; i<=OUT_UP; ++i)
  {
    pinMode(i, OUTPUT);
  }

  cur_step = 0;
  randomSeed(analogRead(0));

}

#define PC_MODE 0
#define USER_MODE 1
#define GAME_OVER 2

int mode = PC_MODE;
int user_step;
int wait_for_input;

void generate_new_random()
{
  steps[cur_step] = random(IN_DOWN, IN_MAX);
  Serial.println(steps[cur_step]);
}

void play_game_over_and_reset()
{
  Serial.println("GAME OVER");
  //maybe not needed
  mode = GAME_OVER;

  for (int i=OUT_FIRST; i<OUT_MAX; ++i)
  {
    digitalWrite(i, LOW);
  }

  for (int i=OUT_FIRST; i<OUT_MAX; ++i)
  {
    digitalWrite(i, HIGH);
    delay(500);
  }

  cur_step = 0;
  mode = PC_MODE;
}

void loop()
{
  if (mode == PC_MODE)
  {
    if (cur_step < MAX_STEPS)
    {
      generate_new_random();
      cur_step++;

      for (int i=0; i<cur_step; ++i)
      {
        Serial.print("writing to");
        Serial.println(steps[i]);
        digitalWrite(steps[i] + IN_TO_OUT, HIGH);
        delay(1000);
        digitalWrite(steps[i] + IN_TO_OUT, LOW);
      }

      mode = USER_MODE;
      user_step = 0;
      wait_for_input = 0;
    }
    else
    {
      //Game won
      mode = GAME_OVER;
    }
  }

  if (mode == USER_MODE)
  {
    if (wait_for_input == 0)
    {
      for (int i=IN_DOWN; i<IN_MAX; ++i)
      {
        int read = digitalRead(i);
        digitalWrite(i+ IN_TO_OUT, read);

        if (read)
        {
          wait_for_input = i;
          Serial.print("read player input:");
          Serial.println(i);
          if (steps[user_step] == i)
          {
            user_step++;
            Serial.println("added step");
            if (user_step == cur_step)
            {
              delay(500);
              mode = PC_MODE;
              Serial.println("All correct. pc mode");
            }
          }
          else
          {
              //GAME OVER
              play_game_over_and_reset();
          }
        }
      }
    }
    else
    {
      if (digitalRead(wait_for_input) == 0)
      {
        wait_for_input = 0;
      }
    }
  }
}
