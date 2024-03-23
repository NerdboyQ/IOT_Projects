$(document).ready(main);

function main(){
    const bt_scan_btn = document.getElementById("bt-scan-btn");
    var bt_device_tbody = document.getElementById("bt-dev-tbody");
    bt_scan_btn.addEventListener('click', run_bt_scan);
    var tbl = document.getElementById("bt-dev-tbl");
    bt_device_tbody.innerHTML = '';

    function run_bt_scan(){
        console.log("clicked");
        bt_scan_btn.disabled = true;
        console.log(bt_scan_btn.disabled);
        bt_device_tbody.innerHTML = '';

        $.getJSON(
            "/scan-for-bt-devices",
            function(data){
                console.log(data);
                for (var key in data){
                    console.log(key);
                    for (var dev of data[key]){
                        console.log(dev);
                        var tr = document.createElement("tr");
                        var bt = document.createElement("bt-dev-container");
                        var h4 = document.createElement("h4");
                        h4.setAttribute('slot', 'bt-dev-name');
                        h4.textContent = dev['name'];
                        var h5 = document.createElement("h5");
                        h5.setAttribute('slot', 'bt-addr');
                        h5.textContent = dev['bt_addr'];
                        bt.appendChild(h4);
                        bt.appendChild(h5);
                        tr.appendChild(bt);

                        bt_device_tbody.appendChild(tr);
                    }
                }
                bt_scan_btn.disabled = false;
            }
        );
    }

}


