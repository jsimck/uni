import { createStructuredSelector } from 'reselect'

export const gamesModalSelector = createStructuredSelector({
    visible: state => state.gamesModal.visible,
    title: state => state.gamesModal.title,
    text: state => state.gamesModal.text,
    type: state => state.gamesModal.type,
    event: state => state.gamesModal.event
});