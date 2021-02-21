import { createSelector } from 'reselect'

export const canvasToolbarSelector = createSelector(
    state => state.canvas.selectedSvgIndexes,
    state => state.maps.processing,
    state => state.canvas.map,
    (selectedSvgIndexes, processing, map) => {
        return {
            firstSelectedSvgIndex: selectedSvgIndexes[0],
            selectedSvgIndexes: selectedSvgIndexes,
            processing: processing,
            grid: map.mapData.grid,
            map: map
        };
    }
);