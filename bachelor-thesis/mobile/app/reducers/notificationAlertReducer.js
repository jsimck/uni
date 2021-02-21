import * as actions from '../actions/notificationAlertActions'

const initialState = {
    alert: {}
};

const notificationAlert = (state = initialState, action) => {
    switch (action.type) {
        case actions.PUSH_ALERT:
            return {
                ...state,
                alert: {
                    title: action.title,
                    message: action.message
                }
            };

        case actions.CLOSE_ALERT:
            return {
                ...state,
                alert: {}
            };

        default:
            return state;
    }
};

export default notificationAlert