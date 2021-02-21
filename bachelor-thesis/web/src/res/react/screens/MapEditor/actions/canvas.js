// Svg manipulation actions
export const UPDATE_SVG = 'UPDATE_SVG';
export const ADD_SVG = 'ADD_SVG';
export const REMOVE_SVG = 'REMOVE_SVG';
export const PUSH_SVG_FORWARD = 'PUSH_SVG_FORWARD';
export const PUSH_SVG_BACKWARD = 'PUSH_SVG_BACKWARD';
export const PUSH_SVG_FRONT = 'PUSH_SVG_FRONT';
export const PUSH_SVG_BACK = 'PUSH_SVG_BACK';

export function addSvg(svg) {
    return {
        type: ADD_SVG,
        svg: svg
    }
}

export function updateSvg(index, svg) {
    return {
        type: UPDATE_SVG,
        index: index,
        svg: svg
    }
}

export function removeSvg() {
    return {
        type: REMOVE_SVG
    }
}

export function pushSvgForward(index) {
    return {
        type: PUSH_SVG_FORWARD,
        index: index
    };
}

export function pushSvgBackward(index) {
    return {
        type: PUSH_SVG_BACKWARD,
        index: index
    };
}

export function pushSvgFront(index) {
    return {
        type: PUSH_SVG_FRONT,
        index: index
    };
}

export function pushSvgBack(index) {
    return {
        type: PUSH_SVG_BACK,
        index: index
    };
}

// SVG Selection actions
export const SET_SVG_SELECTION = 'SET_SVG_SELECTION';
export const ADD_SVG_TO_SELECTION = 'ADD_SVG_TO_SELECTION';
export const REMOVE_SVG_FROM_SELECTION = 'REMOVE_SVG_FROM_SELECTION';
export const CLEAR_SVG_SELECTION = 'CLEAR_SVG_SELECTION';

export function setSvgSelection(index) {
    return {
        type: SET_SVG_SELECTION,
        index: index
    }
}

export function addSvgToSelection(index) {
    return {
        type: ADD_SVG_TO_SELECTION,
        index: index
    }
}

export function removeSvgFromSelection(index) {
    return {
        type: REMOVE_SVG_FROM_SELECTION,
        index: index
    }
}

export function clearSvgSelection() {
    return {
        type: CLEAR_SVG_SELECTION
    }
}

// Maps manipulation actions
export const SET_MAP = 'SET_MAP';

export function setMap(map) {
    return {
        type: SET_MAP,
        map: map
    };
}

// Styles actions
export const UPDATE_GLOBAL_STYLES = 'UPDATE_GLOBAL_STYLES';

export function updateGlobalStyles(styles) {
    return {
        type: UPDATE_GLOBAL_STYLES,
        styles: styles
    };
}

// Grid actions
export const UPDATE_GRID = 'UPDATE_GRID';

export function updateGrid(grid) {
    return {
        type: UPDATE_GRID,
        grid: grid
    };
}

// Clipboard actions
export const CLIPBOARD_CUT = 'CLIPBOARD_CUT';
export const CLIPBOARD_COPY = 'CLIPBOARD_COPY';
export const CLIPBOARD_PASTE = 'CLIPBOARD_PASTE';

export function clipboardCut() {
    return {
        type: CLIPBOARD_CUT
    };
}

export function clipboardCopy() {
    return {
        type: CLIPBOARD_COPY
    };
}

export function clipboardPaste() {
    return {
        type: CLIPBOARD_PASTE
    };
}

// Action actions
export const UPDATE_SVG_ACTION = 'UPDATE_SVG_ACTION';
export const REMOVE_SVG_ACTION = 'REMOVE_SVG_ACTION';

export function updateSvgAction(svgIndex, title, content) {
    return {
        type: UPDATE_SVG_ACTION,
        index: svgIndex,
        title: title,
        content: content
    }
}

export function removeSvgAction(svgIndex) {
    return {
        type: REMOVE_SVG_ACTION,
        index: svgIndex
    }
}