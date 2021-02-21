import { createStructuredSelector } from 'reselect'

export const editStylesOverlaySelector = createStructuredSelector({
    styles: state => state.canvas.map.mapData.styles
});