import * as actions from '../actions/canvasTool'

export const initialState = {
    tool: '',
    toolState: ''
};

const canvasTool = (state = initialState, action) => {
    switch (action.type) {
        case actions.SET_TOOL:
            return {
                ...state,
                tool: action.tool
            };

        case actions.SET_TOOL_STATE:
            return {
                ...state,
                toolState: action.toolState
            };

        default:
            return state;
    }
};

export default canvasTool
