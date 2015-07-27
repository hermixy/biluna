#ifndef RCOMMANDEVENT_H
#define RCOMMANDEVENT_H

/**
 * Command Event.
 *
 * \ingroup core
 */
class RCommandEvent {
public:
    /**
     * Creates a new command event that is not yet accepted.
     *
     * \param cmd the command that was triggered.
     */
    RCommandEvent(const QString& cmd) {
        this->cmd = cmd;
        accepted = false;
    }

    /**
     * \return the command that was triggered, usually by
     * the user.
     */
    QString getCommand() {
        return cmd;
    }

    /**
     * Sets the event state to accepted.
     */
    void accept() {
        accepted = true;
    }

    /**
     * \return Whether the event was already accepted or not.
     */
    bool isAccepted() {
        return accepted;
    }

private:
    QString cmd;
    bool accepted;
};

Q_DECLARE_METATYPE(RCommandEvent*)

#endif
