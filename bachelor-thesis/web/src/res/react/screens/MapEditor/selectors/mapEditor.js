import { createStructuredSelector } from 'reselect'

export const mapEditorSelector = createStructuredSelector({
    notifications: state => state.notifications,
    map: state => state.canvas.map
});