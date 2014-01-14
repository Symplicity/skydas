var config_data = {
  'PANEL_CONFIG': {
    'API_URL': 'http://voip.symplicity.com/api'
  }
}
angular.forEach(config_data,function(key,value) {
  config_module.constant(value,key);
}