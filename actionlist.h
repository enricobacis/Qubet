#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <QList>

/**
 * @brief This class is used to keep track of actions to do.
 *        For example it can keep track of current view scrolling movement, or cube rotations.
 *
 *        There are two type of actions: Primary and Secondary actions.
 *
 *          - Primary action:    You can keep track of only one primary action at a moment,
 *                               it is used for example for the scrolling of menu views,
 *                               only one scrolling at a time.
 *                               When you set a primary action the new one automatically
 *                               overrides the old one.
 *
 *          - Secondary actions: You can keep track of infinite secondary actions at a moment,
 *                               these actions are concurrent toghether and concurrent to the
 *                               primary action.
 *                               They are used for concurrent allowed actions like a
 *                               Seconday Cube Rotation.
 *                               You have to manually remove a secondary action when you
 *                               want to stop it, since appending another secondary action
 *                               will not override the old one (it is a list).
 *
 *        You can have an example of this in Qubet's menu.
 *
 * @version 1.0
 * @author \#34
 */
class ActionList
{

public:

    /**
     * @brief Create a new ActionList with no secondaryActions and 0 as primaryAction.
     */
    explicit ActionList();

    /**
     * @brief Create a new ActionList with no secondaryActions and primaryAction
              given as parameter.
     *
     * @param _primaryAction is the primary action to set to the actionList.
     */
    explicit ActionList(int _primaryAction);

    /**
     * @brief Create a new ActionList as a copy of another ActionList.
     *
     * @param other is the actionList to copy.
     */
    explicit ActionList(ActionList *other);

    /**
     * @brief Safely removes an ActionList.
     */
    ~ActionList();

    /**
     * @brief Set the given parameter as the new primaryAction, overriding the old one.
     *
     * @param _primaryAction is the primaryAction to set.
     */
    void setPrimaryAction(int _primaryAction);

    /**
     * @brief Returns the current primaryAction.
     *
     * @return [int] The current primaryAction.
     */
    int getPrimaryAction();

    /**
     * @brief Appends a secondary action to secondaryActions list.
     *
     * @param _secondaryAction is the secondary action to append.
     */
    void appendSecondaryAction(int _secondaryAction);

    /**
     * @brief Removes a secondary action to secondaryActions list.
     *
     * @param _secondaryAction is the secondary action to remove.
     */
    void removeSecondaryAction(int _secondaryAction);

    /**
     * @brief Returns the secondaryActions list.
     *
     * @return [QList<int>] the secondaryActions list.
     */
    QList<int> getSecondaryActions();

    /**
     * @brief Returns a list of primary and secondary actions.
     *
     *        It is the current secondaryActions list, prepending the primaryAction.
     *
     * @return [QList<int>] a list of primary and secondary actions.
     */
    QList<int> getAllActions();

private:

    int primaryAction; /**< It is the current primary action of the ActionList. */
    QList<int> secondaryActions; /**< It is a list of the current secondary actions of the ActionList */
};

#endif // ACTIONLIST_H
