-- Run this file with MayEngineLua env to prepare ShitLang enviroment
shit = mlog
beng = string
dian = table
budong = math
tobeng = tostring
me.utils.dianToJson = me.utils.tableToJson
package.cpath = package.cpath .. ";./lua/?.dll"
load_shit = fn(chunk)
    local result = me.cfunctions.load_shit(chunk)
    if type(result) == "table" then
        return nil, result[1]
    end
    return load(result, chunk, "b")
end
