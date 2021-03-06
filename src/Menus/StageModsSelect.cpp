#include "Menus/StageModsSelect.h"

std::vector<std::string> getStageModsNames()
{
  string path = std::string(assets_directory) + "mods/stages/";
  return rosalila()->utility->getDirectoryNames(path);
}

std::vector<Image *> getStageModsImages(std::vector<std::string> stage_names)
{
  std::vector<Image *> stage_images;
  for (int i = 0; i < (int)stage_names.size(); i++)
  {
    Image *image = rosalila()->graphics->getImage(std::string(assets_directory) + std::string("mods/stages/") + stage_names[i] + std::string("/images/preview.png"));
    stage_images.push_back(image);
  }
  return stage_images;
}

void stageModsSelect()
{
  std::vector<std::string> stage_names = getStageModsNames();
  std::vector<Image *> stage_images = getStageModsImages(getStageModsNames());

  Image *background = rosalila()->graphics->getImage(std::string(assets_directory) + "menu/white_background.png");
  Image *left_arrow = rosalila()->graphics->getImage(std::string(assets_directory) + "menu/left_arrow.png");
  Image *right_arrow = rosalila()->graphics->getImage(std::string(assets_directory) + "menu/right_arrow.png");

  background->color_filter.green = 0;
  background->color_filter.blue = 0;
  background->width = rosalila()->graphics->screen_width;
  background->height = rosalila()->graphics->screen_height;

  int current_stage = stage_names.size() - 1;
  int frame = 0;

  bool select_button_was_up = false;

  rosalila()->receiver->updateInputs();

  int current_long_press_left = 0;
  int current_long_press_right = 0;

  while (true)
  {
    rosalila()->sound->fadeMusicVolume(128, 2);

    if (rosalila()->receiver->isPressed(0, "back"))
    {
      exit(0);
    }

    if (!rosalila()->receiver->isDown(0, "a"))
    {
      select_button_was_up = true;
    }

    if (rosalila()->receiver->isDown(0, "6"))
    {
      if (current_long_press_right == 0 || (current_long_press_right > 40 && current_long_press_right % 5 == 0))
      {
        rosalila()->sound->playSound("Menu.right", 0, 0, 0);
        current_stage++;
        if (current_stage >= (int)stage_images.size())
        {
          break;
        }
        rosalila()->api_integrator->setStat("current stage", current_stage);
      }
      current_long_press_right++;
    }
    else
    {
      current_long_press_right = 0;
    }

    if (rosalila()->receiver->isDown(0, "4"))
    {
      if (current_long_press_left == 0 || (current_long_press_left > 40 && current_long_press_left % 5 == 0))
      {
        rosalila()->sound->playSound("Menu.left", 0, 0, 0);
        current_stage--;
        if (current_stage < 0)
          current_stage = 0;
        rosalila()->api_integrator->setStat("current stage", current_stage);
      }
      current_long_press_left++;
    }
    else
    {
      current_long_press_left = 0;
    }

    bool error_found = false;

    if (rosalila()->receiver->isDown(0, "a") && select_button_was_up)
    {
      rosalila()->sound->playSound("Menu.confirm", 0, 0, 0);
      rosalila()->utility->writeLogLine("Initializing game.");
      rosalila()->utility->setRandomSeed(time(NULL));
      vector<string> replay_input;
      vector<string> intro_input;

      string game_mode = "Stage select";

      rosalila()->graphics->transparency_effect.set(0, 1);
      rosalila()->graphics->transparency_effect.set(1, 0.03);

      rosalila()->utility->setRandomSeed(time(NULL));

      if (!error_found)
      {
        int current_player_best_score = -1;

        Player *player = new Player("Triangle", 10, intro_input, replay_input, game_mode);
        Enemy *enemy = new Enemy(stage_names[current_stage], player, 20, true);
        Stage *stage = new Stage(player);
        player->enemy = enemy;
        stage->loadFromXML(stage_names[current_stage], true);
        rosalila()->api_integrator->setCurrentControllerActionSet("InGameControls");
        STG *stg = new STG(player, enemy, stage, game_mode, current_player_best_score);

        rosalila()->api_integrator->setCurrentControllerActionSet("MenuControls");

        delete stg;
        delete player;
        delete enemy;
        delete stage;
        select_button_was_up = false;
        rosalila()->graphics->grayscale_effect.set(1, 1);
      }
    }

    rosalila()->graphics->drawImage(background, 0, 0);

    Image *current_stage_image = stage_images[current_stage];

    rosalila()->graphics->drawImage(current_stage_image,
                                      rosalila()->graphics->screen_width / 2 - current_stage_image->getWidth() / 2,
                                      rosalila()->graphics->screen_height / 2 - current_stage_image->getHeight() / 2);

    if (frame % 60 >= 0 && frame % 60 < 30)
    {
      if (current_stage > 0)
      {
        rosalila()->graphics->drawImage(left_arrow,
                                          rosalila()->graphics->screen_width / 2 - current_stage_image->getWidth() / 2 - left_arrow->getWidth(),
                                          rosalila()->graphics->screen_height / 2 - left_arrow->getHeight() / 2);
      }

      if (current_stage < (int)stage_images.size() - 1)
      {
        rosalila()->graphics->drawImage(right_arrow,
                                          rosalila()->graphics->screen_width / 2 + current_stage_image->getWidth() / 2,
                                          rosalila()->graphics->screen_height / 2 - right_arrow->getHeight() / 2);
      }
    }

    rosalila()->update();

    frame++;
  }
}
