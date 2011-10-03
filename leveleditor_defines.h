#ifndef LEVELEDITOR_DEFINES_H
#define LEVELEDITOR_DEFINES_H

// Names used in the LevelEditor View
// This name will be in the format 3xx

#define BUTTON_VOLUME               300
#define LENGHT_DISPLAY              301
#define WIDTH_DISPLAY               302
#define BUTTON_ENLARGE              303
#define BUTTON_REDUCE               304
#define BUTTON_LENGHTEN             305
#define BUTTON_SHORTEN              306
#define LABEL_LENGHT                307
#define LABEL_WIDTH                 308
#define LABEL_SET_LEVEL_NAME        309
#define BUTTON_BACK                 310
#define BUTTON_NEXT                 311
#define FORM_SET_LEVEL_NAME         312
#define TOOLBAR                     313
#define OBSTACLE_0                  314
#define OBSTACLE_1                  315
#define OBSTACLE_2                  316
#define OBSTACLE_3                  317

#define MIN_LEVEL_LENGHT            10
#define MAX_LEVEL_LENGHT            300

#define MIN_LEVEL_WIDTH             3
#define MAX_LEVEL_WIDTH             30

/**
 * @brief Enum used for Editor's actions
 */
enum { // Primary Actions
       DO_NOTHING,
       INITIAL_MOVEMENT,
       GO_TO_SET_NAME_VIEW,
       GO_TO_SET_PARAM_VIEW,
       GO_TO_EDITING_LEVEL_VIEW,

      // Secondary Actions
      ROTATE_VOLUMECUBE, ROTATE_SKYBOX
     };

/**
 * @brief Enum used for Editor's currentView
 */
enum { SET_NAME_VIEW, SET_PARAM_VIEW, OTHER_VIEW, EDITING_LEVEL_VIEW};


#endif // LEVELEDITOR_DEFINES_H
