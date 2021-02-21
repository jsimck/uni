import { createSelector } from 'reselect'

export const canvasSelector = createSelector(
    state => state.canvasMouseEvent.eventMouseMove,
    state => state.canvas.selectedSvgIndexes,
    state => state.canvasTool.tool,
    state => state.canvasTool.toolState,
    state => state.canvas.lastSvgIndex,
    state => state.canvasKeyMap.keyMap,
    state => state.canvas.map,
    (eventMouseMove, selectedSvgIndexes, tool, toolState, lastSvgIndex, keyMap, map) => {
        return {
            eventMouseMove: eventMouseMove,
            selectedSvgIndexes: selectedSvgIndexes,
            activeSvg: map.mapData.svg[lastSvgIndex],
            lastSvgIndex: lastSvgIndex,
            tool: tool,
            toolState: toolState,
            keyMap: keyMap,
            styles: map.mapData.styles,
            grid: map.mapData.grid,
            mapData: map.mapData,
            map: map
        };
    }
);