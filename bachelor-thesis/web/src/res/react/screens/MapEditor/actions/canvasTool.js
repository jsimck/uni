// Tools actions
export const SET_TOOL = 'SET_TOOL';
export const SET_TOOL_STATE = 'SET_TOOL_STATE';

export function setTool(tool) {
    return {
        type: SET_TOOL,
        tool: tool
    };
}

export function setToolState(toolState) {
    return {
        type: SET_TOOL_STATE,
        toolState: toolState
    };
}