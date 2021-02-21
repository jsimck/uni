import React, { Component } from 'react'
import _ from 'lodash'
import { stateToHTML } from 'draft-js-export-html'
import { stateFromHTML } from 'draft-js-import-html'
import { Editor, EditorState, ContentState, RichUtils, convertToRaw, convertFromRaw, CompositeDecorator, Entity } from 'draft-js';
import getDefaultKeyBinding from 'draft-js/lib/getDefaultKeyBinding';
import { RichEditorToolbar } from './index';

function findLinkEntities(contentBlock, callback) {
    contentBlock.findEntityRanges(
        (character) => {
            const entityKey = character.getEntity();

            return (
                entityKey !== null &&
                Entity.get(entityKey).getType() === 'LINK'
            );
        },
        callback
    );
}

const Link = (props) => {
    const {url} = Entity.get(props.entityKey).getData();

    return (
        <a href={url} style={{color: 'blue', textDecoration: 'underline'}}>
            {props.children}
        </a>
    );
};

const decorator = new CompositeDecorator([
    {
        strategy: findLinkEntities,
        component: Link
    }
]);

class RichEditor extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            initialStateSet: false,
            editorState: EditorState.createEmpty(decorator)
        };
    }

    getEntityKeyAtSelection(editorState) {
        var selection = editorState.getSelection();
        var start = selection.getStartOffset();
        var content = editorState.getCurrentContent();
        var contentBlock = content.getBlockForKey(selection.getFocusKey());
        return contentBlock.getEntityAt(start);
    }

    validateUrl(value){
        return /^(https?|ftp):\/\/(((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:)*@)?(((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5]))|((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)+(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.?)(:\d*)?)(\/((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)+(\/(([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)*)*)?)?(\?((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|[\uE000-\uF8FF]|\/|\?)*)?(\#((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|\/|\?)*)?$/i.test(value);
    }

    handleOnAnchorLinkToggle() {
        // Check if link is not already defined on selection
        var entityKey = this.getEntityKeyAtSelection(this.state.editorState);
        if (entityKey != null && Entity.get(entityKey).getType() == 'LINK') {
            // Remove link type entity
            this.setState({
                editorState: RichUtils.toggleLink(
                    this.state.editorState,
                    this.state.editorState.getSelection(),
                    null
                )
            });
        } else {
            // Get url from prompt
            var url = prompt('Zadejte URL:');

            // Validate URL
            if (this.validateUrl(url)) {
                // Create link
                const entityKey = Entity.create('LINK', 'MUTABLE', { url: url });

                this.setState({
                    editorState: RichUtils.toggleLink(
                        this.state.editorState,
                        this.state.editorState.getSelection(),
                        entityKey
                    )
                }, () => {
                    this.focusEditor();
                });
            }
        }
    }

    componentWillReceiveProps(nextProps) {
        if (!_.isNull(nextProps.initialEditorState) && !this.state.initialStateSet) {
            // Set editor state
            this.state = {
                editorState: EditorState.createWithContent(stateFromHTML(nextProps.initialEditorState), decorator),
                initialStateSet: true
            };
        }
    }

    handleOnUtilityClick(utility) {
        switch (utility) {
            case 'undo':
                this.handleOnChange(EditorState.undo(this.state.editorState));
                break;

            case 'redo':
                this.handleOnChange(EditorState.redo(this.state.editorState));
                break;

            case 'link':
                this.handleOnAnchorLinkToggle();
                break;
        }

        // Bring back focus
        this.focusEditor();
    }

    focusEditor() {
        // Set timeout due to editor state batching
        setTimeout(() => {
            this.refs.editor.focus();
        }, 10);
    }

    handleOnToggleInlineStyle(style) {
        // Toggle inline styles on content
        this.handleOnChange(RichUtils.toggleInlineStyle(this.state.editorState, style));

        // Bring back focus
        this.focusEditor();
    }

    handleOnToggleBlockType(block) {
        // Toggle inline styles on content
        this.handleOnChange(RichUtils.toggleBlockType(this.state.editorState, block));

        // Bring back focus
        this.focusEditor();
    }

    handleOnKeyCommand(command) {
        var newState = RichUtils.handleKeyCommand(this.state.editorState, command);

        if (newState) {
            this.handleOnChange(newState);
            return true;
        }

        return false;
    }

    myKeyBindingFunction(e) {
        return getDefaultKeyBinding(e);
    }

    myBlockStyleFn(contentBlock) {
        const type = contentBlock.getType();

        // Apply custom styling
        switch(type) {
            case 'unstyled':
                return 'rich-editor-paragraph';

            case 'header-one':
                return 'rich-editor-header-one';

            case 'header-two':
                return 'rich-editor-header-two';

            case 'header-three':
                return 'rich-editor-header-three';

            case 'ordered-list-item':
                return 'rich-editor-ol';

            case 'unordered-list-item':
                return 'rich-editor-ul';
        }
    }

    handleOnChange(editorState) {
        this.setState({
            editorState: editorState
        });

        // Pass converted HTML content to props
        this.props.onChange(stateToHTML(editorState.getCurrentContent()));
    }

    render() {
        return (
            <div className="rich-editor-wrapper">
                <RichEditorToolbar
                    onToggleBlockType={(value) => this.handleOnToggleBlockType(value)}
                    onToggleInlineStyle={(value) => this.handleOnToggleInlineStyle(value)}
                    onUtilityClick={(value) => this.handleOnUtilityClick(value)}
                />

                <Editor
                    ref="editor"
                    editorState={this.state.editorState}
                    handleKeyCommand={(command) => this.handleOnKeyCommand(command)}
                    blockStyleFn={(block) => this.myBlockStyleFn(block)}
                    keyBindingFn={(event) => this.myKeyBindingFunction(event)}
                    onChange={(editorState) => this.handleOnChange(editorState)}
                />
            </div>
        );
    }
}

RichEditor.propTypes = {
    initialEditorState: React.PropTypes.string,
    editorState: React.PropTypes.string,

    // Functions
    onChange: React.PropTypes.func
};

export default RichEditor