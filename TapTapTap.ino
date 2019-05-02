#include <Gamebuino-Meta.h>

// 1 : LEFT block
// 2 : RIGHT block
int incomingBlocks[5] = { 1, 2, 1, 2, 1 };
int arrowHit = 0;  // 1 left, 2 right, 0 is no arrow hit (waiting for player input)
int score = 0;
int highscore = 0;
int prevHighscore = 0;
int displayHighscore = 0;
int displayPrevHighscore = 0;

void setup() {
  gb.begin();
  prevHighscore = gb.save.get(0);
  if (prevHighscore < 999) {
    prevHighscore = 999;
  }
}

void loop() {
  while (!gb.update());

  // INPUTS //
  if (gb.buttons.released(BUTTON_LEFT) || gb.buttons.released(BUTTON_A)) {
    arrowHit = 1;
  } else if (gb.buttons.released(BUTTON_RIGHT) || gb.buttons.released(BUTTON_B)) {
    arrowHit = 2;
  }

  //reset everything when menu is held for 2 second
  if (gb.buttons.pressed(BUTTON_MENU)) {
    gb.gui.popup("Hold to reset", 50);
  }
  if (gb.buttons.held(BUTTON_MENU, 50)) {
    score = 0;
    highscore = 0;
    prevHighscore = 999;
    gb.save.set(0, prevHighscore);
  }

  // LOGIC //
  // Lower score over time
  score -= score / 200 + 1;
  if (score < 0) {
    score = 0;
  }

  // Did we die?
  if (arrowHit != 0) {
    // Wrong arrow
    if (incomingBlocks[0] != arrowHit) {
      // Lose points
      score -= 25;
      gb.sound.playCancel();
      gb.lights.fill(RED);
    }
    // Correct arrow
    else {
      score += 25;
      gb.sound.playTick();

      gb.lights.fill(BLACK);

      if (score > highscore) {
        highscore = score;
        gb.sound.playOK();
        gb.lights.fill(ORANGE);
      }
      if (highscore > prevHighscore) {
        gb.save.set(0, highscore);
      }

      // Shift everything down
      for (int i = 0; i < 4; i++) {
        incomingBlocks[i] = incomingBlocks[i + 1];
      }
      incomingBlocks[4] = random(1, 3);  // Random new block
    }

    arrowHit = 0;  // reset the hit arrow to 'no arrow pressed'
  }
  // DRAW //
  gb.display.clear();

  int scoreScale = 25;
  // Score
  gb.display.setColor(DARKGRAY);
  gb.display.fillRect(0, gb.display.height() - (score / scoreScale), gb.display.width(), (score / scoreScale));

  // Highscore
  gb.display.setColor(GRAY);
  //lerp easing
  displayHighscore = (15 * displayHighscore + highscore) / 16;
  gb.display.drawFastHLine(0, gb.display.height() - (highscore / scoreScale), gb.display.width());
  gb.display.setCursor(0, gb.display.height() - (highscore / scoreScale) + 2);
  if (highscore > prevHighscore) {
    gb.display.setColor(WHITE);
  }
  gb.display.print(displayHighscore);

  // Previous highscore
  gb.display.setColor(BROWN);
  //lerp easing
  displayPrevHighscore = (15 * displayPrevHighscore + prevHighscore) / 16;
  gb.display.drawFastHLine(gb.display.width() - 20, gb.display.height() - (displayPrevHighscore / scoreScale), 20);
  gb.display.setCursor(gb.display.width() - 20, gb.display.height() - (displayPrevHighscore / scoreScale) + 2);
  gb.display.print(displayPrevHighscore);

  gb.lights.setColor(WHITE);
  if (incomingBlocks[0] == 1) {
    gb.lights.drawPixel(0, 3);
  } else {
    gb.lights.drawPixel(1, 3);
  }
  
  for (int i = 0; i < 5; i++) {
    if (i == 0) {
      gb.display.setColor(WHITE);
    } else {

      gb.display.setColor(BROWN);
    }
    if (incomingBlocks[i] == 1) {
      gb.display.fillRect(25, 40 - (10 * i), 20, 8);
    } else {
      gb.display.fillRect(35, 40 - (10 * i), 20, 8);
    }

  }
}
