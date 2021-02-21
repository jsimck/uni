import * as actions from '../actions/canvasKeyMap'

export const initialState = {
    keyMap: {
        shift: false,
        ctrl: false
    }
};

const canvasKeyMap = (state = initialState, action) => {
    switch (action.type) {
        case actions.KEYMAP_SET_KEY:
            var keyMap = state.keyMap;
            keyMap[action.key] = action.value;

            return {
                ...state,
                keyMap: {
                    ...state.keyMap,
                    ...keyMap
                }
            };

        default:
            return state;
    }
};

export default canvasKeyMap
