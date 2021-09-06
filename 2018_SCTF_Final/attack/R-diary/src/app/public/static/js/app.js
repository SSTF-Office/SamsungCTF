function initTooltip() {
    var elems = document.querySelectorAll('.tooltipped');
    var instances = M.Tooltip.init(elems, {});
}

function initDiaryForm() {
      var elems = document.querySelectorAll('.datepicker');
      var instances = M.Datepicker.init(elems, {});
      $('.materialize-textarea').characterCounter();
}

function initShareForm() {
  var elems = document.querySelectorAll('select');
  var instances = M.FormSelect.init(elems, {});
}

function toast(m) {
   M.toast(m);
}
