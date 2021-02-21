import * as actions from '../actions/notifications'

const initialState = {
    notifications: []
};

// Maintain notification ids
var notificationId = 0;

const notifications = (state = initialState, action) => {
    switch (action.type) {
        case actions.PUSH_NOTIFICATION:
            return {
                ...state,
                notifications: [
                    {
                        id: notificationId++,
                        type: action.notifType,
                        message: action.message
                    },
                    ...state.notifications
                ]
            };

        case actions.REMOVE_NOTIFICATION:
            var index = -1;
            var length = state.notifications.length;

            for (var i = 0; i < length; i++) {
                if (state.notifications[i].id == action.id) {
                    index = i;
                    break;
                }
            }

            return {
                ...state,
                notifications: [
                    ...state.notifications.slice(0, index),
                    ...state.notifications.slice(index + 1)
                ]
            };

        default:
            return state;
    }
};

export default notifications