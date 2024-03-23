/**
 * This file contains my custom html elements
 * 
 * For access to the child elements from any html file 
 * using this element, 'slots' are used for accessing those 
 * respective children.
 */

const template = document.createElement('template');
template.innerHTML = `
    <style>
        /* @import url();  for importing custom css*/
        div {

        }

        /* for the shadow root */
        :host {
            width: 400px;
            height: 150px;
            /* bluish-gray */
            background-color: #446879;
            /* bright blue */
            color: #57A0D3;
            display: flex;
            margin: 10px;
            border: 1px solid #57A0D3;
            border-radius: 15px;
        }
        /* conditional formatting based on the element type */
        :host() {
            
        }
        /* conditional style based on parent element*/
        :host-context() {

        }

        /* anything that's slotted into the element */
        /* NOTE: This is loaded BEFORE the main css is loaded, so
           the changes applied to slotted elements will be overwritten.
           to circumvent this, we use the '!important' key to ensure the
           styling is maintained for this custom element.*/
        ::slotted(h2), ::slotted(h4), ::slotted(h5){
            color: white !important;
            padding: 0 !important;
            margin-left: 15px !important;
        }
        ::slotted(h4){
            margin-bottom: 0 !important;
            margin-top: 15px !important;
        }
        ::slotted(h5){
            margin-top: 0 !important;
        }
    </style>
    <div>
        <slot name="bt-dev-name">Bluetooth Device Name</slot>
        <br>
        <slot name="bt-addr">Bluetooth Address</slot>
        <!-- Gives the implementer the ability to override default styles -->
        <!-- <h5 part="rando">blah blah</h5> -- >
        <img source=/static/img/bt-logo.png alt='bt-logo' />
    </div>
`;
class BluetoothDevContainer extends HTMLElement {
    
    constructor(){
        super();
        this.connected = false;
        console.log("creating bt container")
        // This is the "container/sandbox/canvas" for the custom element
        // opened means the main js can access the internal elements of the element
        // closed means the main js cannot access it's internals
        const shadowRoot = this.attachShadow({mode: 'closed'}); 

        // let div = document.createElement('div');
        // div.textContent = 'BT DEV Container';
        // shadowRoot.append(div);

        let clone = template.content.cloneNode(true);
        shadowRoot.appendChild(clone);
    }

    /* Custom attributes of the element */
    static get observedAttributes() {
        return ['connected'];
    }

    get connected() {
        return this.getAttribute('connected');
    }

    set connected(value) {
        this.setAttribute('connected', value);
    }

    attributeChangedCallback(attrName, oldVal, newVal){
        console.log(attrName + " : " + oldVal + "->" + newVal );
    }
}



/* Custom elements require a hyphen/dash in the name */
customElements.define('bt-dev-container', BluetoothDevContainer);