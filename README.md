# <center>Armor</center>

## <center>Python Virtual Machine</center>

### **Class**

1. class内域的调用。

   在调用类的成员时，会有一个LOAD_ATTR的操作数。而Klass中有一个klass_dict保存这些函数，键是名称，值则是一个FunctionObject。LOAD_ATTR的工作就是根据参数从klass_dict中找到这个函数的FunctionObject再压入栈。

   接下来再有一个CALL_FUNCTION的操作数来调用这个函数，这一步与本地函数无异。
   
2. 在BUILD_CLASS之前的MAKE_FUNCTION和CALL_FUNCTION的作用。

   1. MAKE_FUNCTION：MAKE_FUNCTION對於類和函數都一視同仁，都看作一個codeObject打包進FunctionObject後壓入棧，有參數的話也傳入參數。不夠這個參數是默認參數，一般類的定義沒有默認參數。
   2. CALL_FUNCTION：新建一個frame，傳入這個類的functionObject，args通常為null。新建frame走的是FunctionKlass一路，對於函數來說走這一步是用來賦參以及設置全局變量的，但對於類來說和用codeObject來建一個frame沒有大的區別，對於內部類例外。

3. BUILD_CLASS。

   BUILD_CLASS並不是緊接著上面兩個操作數進行的，如果類裡面有定義的話，會先生成另外一些操作數並執行，比如定義了一個變量，就先LOAD_CONST將值壓入棧，然後STORE_NAME根據參數從names中取一個名字，然後這個名字與值作為鍵值對放入locals中。如果定義了一個方法，就調用MAKE_FUNCTON操作數將函數的codeObject打包成MethodObject作為鍵值對放入locals。

   所有的定義結束後，調用LOAD_LOCALS將locals壓入棧，這個locals就等於是類的屬性包，這個類的對象要調用這個類的屬性都從這個包裡面取。然後RETURN_VALUE將棧中的頂部pop盡上一個frame，本frame結束。

   frame結束後正式進入BUILD_CLASS階段。BUILD_CLASS會從stack中pop出來三個值，分別是locals，class的父類和class的名字。然後通過Klass的create_klass方法創造一個類對象並壓入棧，之所以說是類對象是因為它既是對象也是類。它是TypeObject的實例，自身的klass是TypeKlass，只能通過own_klass綁定一個klass，這個klass才真正包含了類的屬性(包括名字，父類和Attribute)。

   BUILD_CLASS的工作完成後通過一個STORE_NAME的操作正式將類名與這個TypeObject作為鍵值對放入locals。

4. 類的實例化過程。

   類實例化時首先會LOAD_NAME，參數自然是類名，將這個TypeObject壓入棧頂，接著調用CALL_FUNCTION進行實例化的過程。CALL_FUNCTION其實也只是新建了一個frame。根據build_frame的規則，由於TypeObject的klass是TypeKlass類型的，所以會進入allocate_instance()函數。這個allocate_instance具體調用哪一個是由own_klass決定的，一般內建類都有自己的allocate_instance，而自定義的類的own_klass都是Klass的對象，所以調用Klass的allocate_instance。

   這個allocate_instance正式類實例化的關鍵。在這裡會實例化一個PyObject，然後將其的klass設為類的own_klass，這樣對象就順利的繼承了類的所有屬性。並且自身配備了一個obj_dict，可以自己增加新的屬性。

   如果類中定義了初始化函數的話，虛擬機還會在這一步自動調用初始化函數。

5. 調用初始化函數call_virtual

   本質上講init方法和其它方法並沒有本質區別，只是init方法會在類實例化時自動調用。所以init方法也是MethodObject類型的。

6. self關鍵字在虛擬機的作用

   在編譯器中，遇到self關鍵字時會生成一個LOAD_FAST的操作數，這個操作數是用於將調用這個類的對象壓入棧中，然後接下來的LOAD_ATTR中就能知道是哪個類的屬性了。

   所以在定義類的時候，在變量或方法中加入self關鍵字就是為了將其與本類進行綁定。

7. 函數的調用

   對於str或list的諸如upper()或append()類的函數的調用。会事先在相应的klass的klass_dict建立相应的键值对。而對於類的成員函數，在CALL_FUNCTION一步中通过get_attr()函数寻找对应的attribute。如果是对象的成员变量则直接返回。如果是函数就实例化为一个MethodObject，设立的klass也是MethodKlass。这样在build_frame时就会走第二步，第二步的过程主要是在args的头部插入一个owner，这样才能知道这个函数是谁调用的。再build_frame一次，这次走FunctionKlass这一步，正式调用实现实现的函数，并将返回值压入栈。

8. 每個本地函數都要PyObject*，即便本來是void的也要返回一個None。否則無法放入FunctionObject的接收函數指針的構造器中。

9. 每個類其實自己也是一個對象，是TypeObject的實例對象。在自定義一個類時，都是新實例一個klass，然後實例一個TypeObject，將這個klass與type_obj綁定。然後類的一切性質都是由klass決定的，klass只是要依託type_obj來進行運用，因為type_obj是Object的子類，便於在虛擬機內部進行傳遞。

10. 在代碼中定義一個類後，虛擬機並不會立刻加載一個類，而是將這段代碼作為一個codeObject加載入consts，同時也包括了類的名字。只要當類實例化時，這個類的名字和類的codeObject同時加載入操作數棧，作為一個函數來加載。**這就是為什麼python的類實例化的語法和調用函數的語法是一樣的。**

11. 所以在定義一個類的時候，就有一個MAKE_FUNCTION和CALL_FUNCTION的調用，MAKE_FUNCTION是將類名與對應的codeObject進行綁定放入locals，CALL_FUNCTION就是調用Klass::create_klass()了，解析codeObject來生成一個自定義類的TypeObject並壓入棧。

12. store_name那裡還是用的____main____，要改一下。

13. 類的實例化過程。在類實例化時，編譯器產生的是一個CALL_FUNCTION的參數，然後就會產生一個棧幀frame。在這個frame中，此時棧頂的應該是在類定義時壓入棧的一個TypeObject，然後在build_frame函數中會符合TypeKlass::get_instance()，所以會調用klass的一個allocate_instance函數，這個函數就是負責根據類名，來生成一個PyObject*類型的對象，並將TypeObject的own_klass賦為這個object的klass，因為類的屬性都放在了這個own_klass的klass_dict中，所以這個新生成的對象通過調用自身klass的klass_dict就可以享有類的全部屬性。

14. 在python中，自定義類型可以動態地添加屬性，即在引用一個屬性時，如果類中沒有定義這個屬性，則會自動添加並賦值。對於內建類型則不行。

15. 初始化函數或者說構造函數____init___的定義並沒有特定的字節碼，這是在虛擬機內部發起的。通過類型來創造對象時，就要檢查是否定義了這個函數，如果有則需要調用。調用的步驟是在allocate_instance裡面進行的。

16. order_super()的思路：對於某個類型，直接遍歷它的所有父類，合併其父類的mfo(Method Find Order)，這個mfo是一個放有TypeObject指針類型的List。而遍歷的原則是深度優先遍歷，即先遍歷某個一級父類。一直遍歷這個一級父類的祖先類。然後保留所有不重複的屬性。如果有重複的元素就將其刪除，只保留該元素的最後一次出現，即實現了子類對於父類的重載。通俗來講，對於子類的第一級父類之間，確保在使用它們的共同父類的屬性之前，其下所有的子類都沒有重載這個屬性。

    **所以這裡有個非常容易誤解的地方，雖然是使用的深度優先遍歷來實現這個排序，但是類的繼承表現出來的卻是廣度優先遍歷，或者說層級遍歷。**

### **Function**

1. fast_locals釋義：fast_locals，單憑名字很難知道這個ObjList類型的對象時幹什麼的，其實這是用於存儲外界賦予這個函數的局部變量(主要就是函數的參數)。函數內部定義的局部變量可以用locals存儲，但是外部賦予的局部變量的調用方式不一樣，所以不能本地局部變量放在一起，所以另外定義一個fast_locals。
2. 形參的調用(沒有默認值的情況)：形參對於函數唯一的意義就是形參的排列順序。在函數內部的定義中遇到形參時，編譯器會自動生成一個LOAD_FAST操作數，這個操作數的參數就是形參在fast_locals中的index，於是就可以直接根據index將形參的值調入棧中，然後根據操作數進行計算，再將結果壓入棧。
3. 默認參數的輸入：在有默認參數的情況下，MAKE_FUNCTION的參數便是默認參數的個數，然後從棧中pop出這麼多數作為args傳入functionObject的實例化參數，然後functionObject就會在自己的_defaults域存入這些數。當調用這個函數時，首先是將這些默認參數按照倒序傳入fast_locals中，再將調用函數時的參數按照正序傳入fast_locals中，可能會對默認參數形成覆蓋，但總是位於前面的默認參數先被覆蓋。基於這種機制，python語法中設置了在有默認參數的情況下，默認參數只能全部放在必選參數的後面。

### **Garbage Collection**

1. Tracing GC算法：以roots集合為起始點進行圖的遍歷，如果roots到某個對象是可達的，就是“可達對象”。否則就是“不可達對象”，需要被回收。CPython正是使用的這種垃圾回收機制。roots的定義是所有本身不在堆裡而指向堆內對象的引用的集合。棧裡面的對象都是roots，注意這裡的棧指代內存中的一塊區域，和數據結構的棧並不是一回事。實際上就可以理解成frame裡面的stack，stack裡面放的都是各個對象的引用，這就是棧空間。

   * mark階段：根據對象的引用關係在整個圖中進行引用，如果能夠訪問到就mark為活的，而不可到達的對象就沒有mark。

   * 內存回收階段：回收階段，如果對象被標記了，就只將mark的標記清除，如果沒有被標記，就將這個對象的起始地址和大小記錄到一個鏈錶freelist中。

     在freelist中，會首先檢查鏈錶的末尾的地址，如果和傳入對象的起始地址相鄰，說明是一塊共同的內存區域，可以與鏈錶末尾合在一起回收。否則就掛在鏈錶的末端。

   * 內存分配階段：如果僅僅將這些沒有引用的對象回收起來當然不能起到節約內存的作用，在為新的對象分配內存時，可以優先考慮從freelist中選擇內存區域。分配內存的方式有點類似於操作系統的動態分配內存的方式，建議去看看。

2. Copy GC算法：這個算法是我們的虛擬機最終選擇的算法。Copy GC將堆空間分為相同的兩半，分別為from space(allocation space)和to space(survivor space)。當from space中的空間不夠分配對象時，就會觸發GC，將from space所有的活對象換到to space中，這樣from space中的不可追蹤到的對象的空間就可以任意使用了。

   Copy GC算法實現：觸發GC時，先是將所有roots裡面的對象搬到to space中去，並且為每個搬過去的對象都設置一個forwarding屬性表示新地址，這樣如果有兩個對象都引用了同一個對象，後一個對象就知道自己引用的對象搬去了哪裡，自己就只要改一下引用的指針就可以了。當所有可訪問的對象都搬走了以後，剩下的對象就都是可以回收的垃圾了。
   
3. new的重載：為了使new出來的對象都在堆空間中產生，對new關鍵字進行重載。

4. PyString的內存分配：對於char類型的數組無法進行new的重載，為了避免內存洩露。使用不同的內存分配方式。

5. Placement New:又稱為定位new，定位new可以讓我們在一塊分配好的內存空間構造一個對象。由於ArrayList和Map中使用了大量的模板操作，不知道模板T的大小也就無法直接重載new。所以正好使用定位new來合理分配內存。

   定位new的語法為：

   ```c++
   new(place_address)type; //new an instance
   new(place_address)type[n]; //new an array
   ```

   這兩個函數都會返回對象或數組的首地址。

6. mark_word的設置：作者將mark_word放在PyObject的第一個位置，就是為了使得其內存偏移為0，而由於在堆中保證了每個對象都是8字節對齊的，所以long類型的mark_word的低三位均為0(由於編譯器的內存對齊作用)。所以這三位可以用來存儲在圖遍歷過程中有關的訪問次數的問題。

   在第一次copy對象到to space時需要設置一個新地址，此時做了一次與0x2做或運算的操作，這就使得低三位的第二位被置為1。所以第2位為1的地址就是從來沒有訪問過的對象。

   而在new_address()方法中，巧妙地利用了這個置第2位為1的操作。首先由於內存對齊，只有第2位為1的地址才是放入堆內的合理的地址。所以可以根據這一點判斷地址是否合法。然後對地址執行了一次與-8的與運算，使得低三位重新被置為0。

   所以垃圾回收很大程度上寄託在了這個mark_word放置在PyObject的第一位上，所以非常重要。

### **Copy GC的實現過程**

1. scavenge方法：scavenge是整個GC算法的入口，先來看它是怎麼實現的。scavenge裡面非常簡單，只有兩步：先處理roots，然後處理_stack裡面的對象。

2. do_oop方法：在scavenge的兩步實現之前，要先講講do_oop方法。do_oop方法其實要和copy_and_push方法放在一起講。

   在do_oop方法中，會首先檢查傳入的obj是否為null和是否在from空間中。如果都滿足，就執行copy_and_push方法。

   copy_and_push方法：在這個方法中，會首先訪問傳入對象的new_address，因為一個對象有多個對象引用，可能已經有其他root事先將這個對象搬過去了。如果new_address為null，說明沒有copy過。然後會先分配內存，將對象的一切都copy過去，set_new_address的點之前講過了。然後壓入棧中，待之後遍歷其它對象使用。然後返回新地址。

3. process_roots:

   * Universe::oops_do: 這個方法處理的都是一些系統內獨一無二的數據如true，false等。這些數據基本不會進入垃圾回收，只會搬來搬去。
   * Interpreter::oops_do: 搬動_builtins和__ret_value等數據。然後進入frame的搬動。frame搬動過程中是從最裡面的一個frame開始向外遞歸，最後到最外面的一個frame。
   * StringTable的一些常量不多講。

4. ScavengeClosure::do_arraylist():對於arraylist的複製方法。由於arraylist裡面儲藏的都是klass或者object的指針，所以先將指針數據複製到to空間。然後在arraylist中存在一個存儲數據的數組，由於arraylist類中只存儲了這個數組的首指針。也就是說之前只是將這個指針的地址複製了過去，數組內包含的每個數據的指針還沒有。所以在ArrayList中設立了一個do_oop方法，這個方法首先將數組的每個數據的指針複製了過去，然後根據這些指針將每個被引用的對象也複製了過去。於是這一步就完成了。

   對於klass類和object類的不同：klass類直接do_klass後轉換到klass類的oops_do之中將klass類的所有複製過去就完事了。而object類會進入do_oop類，在這裡會檢查是否是from空間內的類，然後進入copy_and_push。copy過程中，會佔用其對應klass大小的空間。同時set一個新的address，複製過程便可看成第一次的訪問過程，因此在set_new_address時，mark_word被置為新的address。同時通過與0x2或運算使第2位置為1.
   
5. 對於壓入棧中的對象，對它們都調用oops_do()方法。oops_do方法中就是先複製包含有自定義屬性的obj_dict,這一點主要是針對自定義對象。然後就調用對象的klass的do_oop方法。內建類的do_oop方法根據特定的klass各有不同，而自定義類統一調用klass的do_oop方法——就是將klass的所有屬性都一一複製過去。但不是簡單的複製，而是調用closure的do_oop方法，這個方法除了複製這些屬性外，還會將這些屬性再壓入棧。因為這些屬性本質上也是對象。所以就形成了一個深度優先遍歷的模式。直到遇到一個對象，它的所有屬性都沒有其他對象引用。

### **加載動態庫**

1. dlopen

   Linux下有一組函數進行動態庫的加載和關閉

   dlopen用於打開動態庫，其原型為：

   ```c
   void* dlopen(const char* filename, int flag);
   filename代表加載庫的文件名。
   flag代表代表函數符號的解析方式。
   RTLD_LAZY代表延遲綁定，只在使用的時候才解析。
   RTLD_NOW在庫加載時就完成所有的函數綁定工作。
   ```

2. dlsym

   dlopen執行後會返回一個句柄handle(可以看成是一個智能指針)，多次調用會返回同一個句柄，可以看成是存儲動態庫的地址。dlsym的作用就是根據符號名找到需要的符號，其原型為：

   ```c
   void* dlsym(void* handle, char* symbol);
   ```

3. dlclose

   根據dlopen返回的句柄來關閉動態庫。
   
4. 先講一下為什麼要設置這個動態庫，首先是可以直接在python代碼中通過import + 動態庫名的方式導入C++動態庫，齊次是為了和python本來的math庫兼容，可以自建一個math.py文件，然後在那個文件中導入用c++寫的庫，這樣就實現c++庫的透明，用戶不需要知道哪些庫是用c++寫的。但c++庫一般封裝的都是函數，如果是變量如π之類的就直接math.py中定義。

5. 導入動態庫的流程，以libmath舉例：

   首先在inc/armor.hpp定義了一個結構體RGMethod,其包含了被封裝函數的基本信息。然後在lib/math.cpp中定義好需要用的函數如sqrt等。然後將這些函數放入一個RGMethod數組中，通過init_libmath方式可以返回這個數組。可能是dlsym無法直接找到這個數組的原因，必須先找到這個函數，再通過這個函數找到這個數組。找到了這個函數數組，接下來只需要將它們放入module就好了。


### **Universe::genesis() - 系統初始化函數**

genesis用於初始化整個系統，其過程包括：

1. 得到ObjectKlass的實例，ObjectKlass構造器未實現任何東西。
2. 得到TypeKlass的實例，構造器也未實現任何東西。
3. 得到TypeObject的實例，構造器set了一個klass


