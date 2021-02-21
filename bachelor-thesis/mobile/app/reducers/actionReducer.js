import * as actions from '../actions/actionActions'

const initialState = {
    action: {}
};

const event = (state = initialState, action) => {
    switch (action.type) {
        case actions.SET_ACTION:
            return {
                ...state,
                action: {
                    ...action.action
                }
            };

        default:
            return state;
    }
};

export default event